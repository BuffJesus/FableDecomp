"""Hands-free playtest driver: installs tools/script_recovery/ingame_runner.lua (per-frame reflexes: health,
hostile clearing, follower-paced route hops) as a thread of an always-alive quest host, then plans and
watches from here without a model in the loop.

    python tools/script_recovery/ingame_runner.py tools/script_recovery/runner_quests/trader_escort.json \
        --bundle v15 --tag te1 [--minutes 30]

Quest file (JSON):
  host       quest VM the runner lives in (LUAGameflow is alive for the whole game)
  maps       regex over map / region names whose LEVs the planner loads
  legs       {region: [x, y] | [x, y, z] | [x, y, z, false]} -- where to walk in each region (with a z: its onward
             region exit, retried until it fires; `false` = an exact spot that is not an exit)
  enemyNames script names the runner clears even when the area query misses them
  fightNames script names a quest needs killed BY the hero (it waits on MsgIsKilledBy): drained to 1 health,
             then a real attack (sword clicks, every 4th a spell) lands the last blow
  fightDefs  definitions killed the same way, before fightNames (a boss's summoned minions: its AI counts
             their deaths, and a fade is not one)
  fightDrain false to use real damage throughout the fight (default true)
  fightBounds [minX, minY, maxX, maxY]: keep combat targets and landing spots inside this rectangle
  fightInputs {scriptName: [input, ...], "default": [input, ...]}: override attack inputs for a target
  fightWhen  optional Lua condition in stateHost (default host), checked before combat input
  stateStatus optional Lua expression in stateHost, logged every third poll for quest-specific diagnostics
  hunt       true: with no route, walk to the nearest living enemyNames enemy
  clear      false: no reflex clearing of hostiles near the hero (a disguise quest: Bandit Camp's gate guard is a
             hostile until the disguise is worn, and clearing him ended his entity script)
  answer     "yes" | "no": the default answer to a yes/no question (frames pause while it is up)
  steps      optional [{name, travel, fight, talk, talkLabel, until, stateHost, timeout, tries, pauseSteps}, ...]
             after the start: cross to `travel` ([slot, x, y, z, region] like start.travel), fight the `fight`
             script names for the step's duration, talk to the script-named NPC, then wait until the Lua expression
             is true (a step's pauseSteps answers its own yes/no questions); still false after `tries` fails the run
  pauseSteps optional input ladder for paused frames instead of [answer, clear, answer, ENTER] (e.g. ["clear"] where
             a scripted conversation pauses frames and a mouse answer would be an attack)
  done       Lua expression, evaluated in `host`, true when the test succeeded
  failed     Lua expression, true when it failed
  party      Lua expression (in `partyHost`) for the followers the quest expects right now
  start      optional: take the quest at the Guild card table and travel to it, so a run starts BEFORE the card
             {"quest": "Q_X", "title": "<card title as the table shows it>", "guild": [slot, x, y, z],
              "card": "OBJECT_QUEST_CARD_<any card on the table>", "travel": [slot, x, y, z]}
             (skipped when the quest is already active; the card is taken through the real UI: TAB beside a card,
             the row found by OCR of the summary title while hovering each row, then Take Quest)
             optional "talk": "<script name>", "talkLabel": "<name shown when targeted>": start the conversation
             the quest waits for (a quest Gameflow grants without a card leaves out "card"/"title"); an empty
             talkLabel presses TAB once facing them without the OCR check (logged `talk_blind`)
             optional "setup": [Lua statement, ...] run once in `host` before travel (assistance; item boxes cleared),
             "setupCheck": a Lua expression that must be true after it
Log: work/runner/<tag>.jsonl (one status per poll) + screenshots at stalls and pauses.

    --launch --save PROFILE   stage PROFILE's AutoSave, launch the bundle and load it (autopilot's launcher);
                              the game closes before the staged-over profile is restored when the runner ends
    --harvest PROFILE         after a `done` finish, keep the game's newest AutoSave as PROFILE
    --close                   close Fable at the end (automatic with --save)
"""
from __future__ import annotations

import argparse
import json
import math
import re
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.autopilot import (Channel, game_input, stage_save, restore_save, harvest_save,  # noqa: E402
                                             launch_and_load, log_path, SAVES, FRONTEND_PROFILE_DIR)
from tools.script_recovery.walkgrid import Grid  # noqa: E402
from tools.script_recovery.ab_playtest import fable_running  # noqa: E402

RUNNER_LUA = Path(__file__).with_name('ingame_runner.lua')
# things the planner routes around everywhere (TNG definitions): Darkwood's exploding spores
HAZARDS = {'OBJECT_EXPLODING_SPORE_MEDIUM', 'OBJECT_EXPLODING_SPORE_LARGE'}
OCR = Path(__file__).with_name('ocr_png.ps1')
# nearest living enemy by the quest's script names: "x,y,distance" or "none"
HUNT_EXPR = ("(function() local h = quest:GetHero(); local best, bd; for _, n in ipairs(Runner.enemyNames) do "
             "for _, e in ipairs(quest:GetAllThingsWithScriptName(n) or {}) do "
             "if e:IsAlive() and quest:GetHealth(e) > 0 then local d = quest:GetDistanceBetweenThings(h, e); "
             "if not bd or d < bd then bd, best = d, e end end end end; "
             "if not best then return 'none' end; local p = best:GetPos(); "
             "return string.format('%.1f,%.1f,%.1f', p.x, p.y, bd) end)()")
# the card-table screen at the game's 1024x768 window: list rows (the first at y 133, 25 px apart), the summary
# title that names the hovered row, and the row menu the engine shows for a chosen quest (Take Quest first)
CARD_ROW_X, CARD_ROW_Y, CARD_ROW_STEP, CARD_ROWS = 320, 133, 25, 10
CARD_TITLE_BAND = (420, 455)
TAKE_QUEST = (300, 133)
CARD_SIDES = [(-1.5, 0), (0, 1.5), (0, -1.5), (-1.1, 1.1), (-1.1, -1.1), (2, 0)]   # stand-offs from a card, tried in order


def ocr_lines(png: Path) -> list[tuple[int, int, str]]:
    """(x, y, text) per line Windows' OCR finds in the image"""
    import subprocess
    r = subprocess.run(['powershell', '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', str(OCR), str(png)],
                       capture_output=True, text=True, encoding='utf-8', errors='replace')
    out = []
    for line in r.stdout.splitlines():
        box, _, text = line.partition('\t')
        parts = box.split()
        if len(parts) == 4 and text:
            out.append((int(parts[0]), int(parts[1]), text.strip()))
    return out


def norm(s: str) -> str:
    return re.sub(r'[^a-z0-9]', '', s.lower())


def target_labels(png: Path, expected: str) -> list[str]:
    """Read the small outlined target name against a moving scene background."""
    labels = [t for x, y, t in ocr_lines(png) if 350 <= x <= 675 and y < 45]
    if any(norm(t) == norm(expected) for t in labels):
        return labels
    from PIL import Image
    with Image.open(png) as screenshot:
        crop = screenshot.crop((350, 0, 675, 48)).resize((1300, 192)).convert('L')
    # Keep a full-sized canvas: Windows OCR missed the same isolated word on
    # a short strip. Extract the white glyphs to remove sky and outline noise.
    for threshold in (220, 210, 200):
        glyphs = crop.point(lambda p: 0 if p > threshold else 255).resize((650, 96))
        canvas = Image.new('RGB', (1024, 768), 'white')
        canvas.paste(glyphs, (300, 300))
        processed = png.with_name(png.stem + f'_target_{threshold}.png')
        canvas.save(processed)
        labels.extend(t for _, _, t in ocr_lines(processed))
        if any(norm(t) == norm(expected) for t in labels):
            break
    return labels


def parse_status(s):
    out = {}
    for k, v in re.findall(r'(\w+)=(\S+)', s or ''):
        out[k] = v
    if 'pos' in out:
        out['pos'] = tuple(float(t) for t in out['pos'].split(','))
    return out


def settle_world(scene_ready, *, timeout=180, report=lambda **_: None):
    """Drain arrival tutorials and observe two clear, unpaused scene checks.

    A loaded region alone is insufficient: another crossing can otherwise
    arrive while a first-time tutorial is still being queued.
    """
    deadline, quiet = time.monotonic() + timeout, 0
    while time.monotonic() < deadline:
        cleared = game_input('clear').strip() != '(no box)'
        ready = scene_ready()
        quiet = quiet + 1 if ready and not cleared else 0
        report(cleared=cleared, ready=ready, quiet=quiet)
        if quiet >= 2:
            return True
        time.sleep(2)
    return False


def archive_run_log(bundle, tag):
    import shutil
    source = log_path(bundle)
    if source.is_file():
        destination = ROOT / 'work/runner' / f'{tag}_FableScriptExtender.log'
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, destination)
        print(f'archived game log -> {destination}', flush=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('quest', type=Path)
    ap.add_argument('--bundle', default='v15')
    ap.add_argument('--tag', default='run')
    ap.add_argument('--minutes', type=float, default=30)
    ap.add_argument('--launch', action='store_true', help='launch the bundle and load the staged save first')
    ap.add_argument('--save', metavar='PROFILE', help='stage this profile\'s AutoSave before launching')
    ap.add_argument('--harvest', metavar='PROFILE', help='after a done finish, keep the newest AutoSave as PROFILE')
    ap.add_argument('--close', action='store_true', help='close Fable when the run ends (for chained runs)')
    a = ap.parse_args()
    if a.save and not a.launch:
        ap.error('--save requires --launch')
    # Refuse before staging or entering cleanup: a rejected launch must neither
    # overwrite a live session's save nor close its game through --close.
    if a.launch and fable_running():
        ap.error('Fable.exe is already running; close it before launching a replay')
    q = json.loads(a.quest.read_text(encoding='utf-8'))
    live = SAVES / FRONTEND_PROFILE_DIR / 'AutoSave'
    backup = stage_save(a.save) if a.save else None
    try:
        baseline = live.stat().st_mtime if live.is_file() else 0.0
        if a.launch:
            launch_and_load(a.bundle)
        result = play(a, q)
        if a.harvest and result == 'done':
            if not harvest_save(a.harvest, a.save, baseline):
                raise RuntimeError('quest completed but no new autosave was harvested')
    finally:
        if a.close or backup:
            import subprocess
            subprocess.run(['taskkill', '/IM', 'Fable.exe', '/F'], capture_output=True)
            time.sleep(3)
        # restored only now: the game autosaves into the staged-over profile for as long as it plays
        try:
            archive_run_log(a.bundle, a.tag)
        finally:
            restore_save(backup)
    sys.exit(0 if result == 'done' else 1)


def play(a, q) -> str:
    host = q.get('host', 'LUAGameflow')
    out = ROOT / 'work' / 'runner'
    out.mkdir(parents=True, exist_ok=True)
    logf = (out / f'{a.tag}.jsonl').open('a', encoding='utf-8')
    c = Channel(a.bundle)
    grid = Grid(q['maps'], hazards=HAZARDS | set(q.get('hazards', [])))
    world = {name: region for name, (_, _, _, _, region) in grid.maps.items()}

    def send(line, timeout=10):
        return c.send([line], timeout=timeout)

    def ev(expr, timeout=10):
        reply = send(f'eval {host}: {expr}', timeout)
        val = next((r.rsplit(' = ', 1)[-1] for r in reply if '[Autopilot] eval ' in r), None)
        err = next((r for r in reply if 'error' in r.lower() and '[Autopilot]' in r), None)
        return val, err

    def state_eval(expr, timeout=10):
        reply = send(f"eval {q.get('stateHost', host)}: {expr}", timeout)
        return next((r.rsplit(' = ', 1)[-1] for r in reply if '[Autopilot] eval ' in r), None)

    def log(kind, **kw):
        logf.write(json.dumps({'t': round(time.time(), 1), 'kind': kind, **kw}) + '\n')
        logf.flush()
        print(kind, json.dumps(kw)[:220], flush=True)

    # install: the file loads with the HOST environment as _ENV (the chunk's own env falls back to it)
    path = str(RUNNER_LUA).replace('\\', '/')
    install = (f"{host}: local E = (getmetatable(_ENV) or {{}}).__index or _ENV; local f = assert(loadfile('{path}', 't', E)); f(); "
               f"E.Runner.enemyNames = {{{', '.join(repr(n) for n in q.get('enemyNames', []))}}}; "
               f"E.Runner.partyNames = {{{', '.join(repr(n) for n in q.get('partyNames', []))}}}; "
               f"E.Runner.fightNames = {{{', '.join(repr(n) for n in q.get('fightNames', []))}}}; "
               f"E.Runner.fightDefs = {{{', '.join(repr(n) for n in q.get('fightDefs', []))}}}; "
               f"E.Runner.fightDrain = {'true' if q.get('fightDrain', True) else 'false'}; "
               f"E.Runner.clear = {'true' if q.get('clear', True) else 'false'}; "
               f"E.Runner.fightBounds = " + ('{' + ','.join(str(float(v)) for v in q['fightBounds']) + '}' if q.get('fightBounds') else 'nil') + '; ' +
               f"if not E.Runner.thread then E.Runner.thread = true; quest:CreateThread('RunnerMain') end")
    regions = ', '.join(f'["{m}"]="{r}"' for m, r in world.items() if m in grid.maps and re.search(q['maps'], m + ' ' + r))
    # the game reads a batch only while script frames run, and the next send replaces an unread one: a lost
    # install left wb2 with no runner at all (and no error to see). Install and region table go as one batch,
    # then the runner's presence is checked and the batch resent until it is there.
    for attempt in range(12):
        reply = c.send([install, f'{host}: Runner.regionOf = {{{regions}}}'], timeout=20)
        present, _ = ev("tostring(Runner ~= nil and Runner.regionOf ~= nil and next(Runner.regionOf) ~= nil)", 10)
        log('install', attempt=attempt, present=present, reply=[r for r in reply if 'error' in r.lower()][:2])
        if present == 'true':
            break
        game_input('clear')
        time.sleep(3)
    else:
        return 'failed'

    def wait_for(expr, want, seconds):
        end = time.time() + seconds
        while time.time() < end:
            val, _ = ev(expr, 6)
            if val == want:
                return True
            game_input('clear')             # a region's arrival boxes pause script frames
            time.sleep(2)
        return False

    def travel(dest):
        """one retail map-slot crossing [slot, x, y, z] or a chain of them, each [slot, x, y, z, "Region"] waiting
        for its region (Q_WaspBoss waits on Lookout Point BEFORE it binds anything, then on the Picnic Area)"""
        for hop in (dest if isinstance(dest[0], list) else [dest]):
            slot, x, y, z = hop[:4]
            ready = lambda: ev("tostring(not quest:IsInCutscene() and not quest:IsInMovieSequence())", 6)[0] == 'true'
            if not settle_world(ready, report=lambda **state: log('travel_ready', **state)):
                log('travel_not_ready', dest=hop)
                return False
            send(f"{host}: assert(not quest:IsInCutscene() and not quest:IsInMovieSequence(), 'crossing refused: scene active'); "
                 f"quest:GoToMapSlotRetailTransition({slot}, {x}, {y}, {z})", 30)
            log('crossing', dest=hop)
            # the hero near the target first: a region can already be loaded before the crossing (the Guild's
            # exterior map belongs to LookoutPoint, so wb2 fired the Picnic crossing six seconds after this one)
            near = (f"(function() local p = quest:GetHero():GetPos(); "
                    f"return tostring(math.abs(p.x - {x}) < 40 and math.abs(p.y - {y}) < 40) end)()")
            if not wait_for(near, 'true', 240):
                log('crossing_not_arrived', dest=hop)
                return False
            if len(hop) > 4 and not wait_for(f"tostring(quest:IsRegionLoaded('{hop[4]}'))", 'true', 240):
                log('region_not_loaded', region=hop[4])
                return False
        return True

    def take_card(s):
        """the real card table: TAB beside a card, find the quest's row by the summary title, Take Quest"""
        card = s['card']
        if not wait_for(f"tostring(#quest:GetAllThingsWithDefName('{card}') > 0)", 'true', 5):
            if not travel(s['guild']):
                return False
            if not wait_for(f"tostring(#quest:GetAllThingsWithDefName('{card}') > 0)", 'true', 200):
                log('card_table_missing', card=card)
                return False
        # TAB acts on the current target, and the Guildmaster stands beside the table: from the card's +x side
        # he is the target (his name shows top centre) and TAB talks to him (wb1). Stand where no one's name shows.
        for dx, dy in CARD_SIDES:
            send(f"{host}: local c = quest:GetAllThingsWithDefName('{card}')[1]; local p = c:GetPos(); "
                 f"p.x = p.x + {dx}; p.y = p.y + {dy}; local h = quest:GetHero(); "
                 f"quest:EntityTeleportToPosition(h, p, 0, true, true); quest:EntitySetFacingAngleTowardsThing(h, c, true)", 20)
            time.sleep(1.5)
            png = out / f'{a.tag}_card_side.png'
            game_input(f'capture {png}')
            target = [t for _, y, t in ocr_lines(png) if y < 45]
            log('card_side', side=[dx, dy], target=target)
            if not target:
                break
        game_input('key TAB')
        time.sleep(1.5)
        game_input('clear')                 # the inventory tutorial box, the first time the screen opens
        time.sleep(1.0)
        png = out / f'{a.tag}_card_screen.png'
        game_input(f'capture {png}')
        if not any(norm(t) == 'availablequests' for _, _, t in ocr_lines(png)):
            log('card_screen_missing')
            return False
        found, prev = None, None
        for k in range(CARD_ROWS):
            game_input(f'move {CARD_ROW_X} {CARD_ROW_Y + k * CARD_ROW_STEP}')
            time.sleep(0.6)
            png = out / f'{a.tag}_card_row{k}.png'
            game_input(f'capture {png}')
            titles = [t for _, y, t in ocr_lines(png) if CARD_TITLE_BAND[0] <= y <= CARD_TITLE_BAND[1]]
            log('card_row', row=k, title=titles)
            if titles and norm(titles[0]) == norm(s['title']):
                found = k
                break
            if k and titles == prev:        # hovering past the last row leaves the title unchanged
                break
            prev = titles
        if found is None:
            game_input('key ESC')
            log('card_not_listed', title=s['title'])
            return False
        game_input(f'click {CARD_ROW_X} {CARD_ROW_Y + found * CARD_ROW_STEP}')
        time.sleep(1.0)
        game_input(f'click {TAKE_QUEST[0]} {TAKE_QUEST[1]}')
        time.sleep(2.0)
        game_input('key ESC')
        time.sleep(2.0)
        return wait_for(f"tostring(quest:IsQuestActive('{s['quest']}'))", 'true', 30)

    def talk(name, label):
        """start a conversation the quest waits for: stand beside the script-named NPC until the top-centre target
        label is theirs (OCR; TAB acts on whoever is targeted), then TAB"""
        wait_for("tostring(not quest:IsInCutscene() and not quest:IsInMovieSequence())", 'true', 180)
        for dx, dy in CARD_SIDES:
            send(f"{host}: local g = quest:GetThingWithScriptName('{name}'); local p = g:GetPos(); "
                 f"p.x = p.x + {dx}; p.y = p.y + {dy}; p.z = p.z + 0.5; local h = quest:GetHero(); "
                 f"quest:EntityTeleportToPosition(h, p, 0, true, true)", 20)
            # Teleport is applied at an engine boundary and can overwrite facing
            # set in the same batch. Turn only after the hero has landed.
            time.sleep(1.0)
            send(f"{host}: local g = quest:GetThingWithScriptName('{name}'); "
                 f"quest:EntitySetFacingAngleTowardsThing(quest:GetHero(), g, true); "
                 f"quest:CameraResetToViewBehindHero(0.1)", 20)
            time.sleep(1.5)
            png = out / f'{a.tag}_talk_{dx}_{dy}.png'
            game_input(f'capture {png}')
            target = target_labels(png, label)
            log('talk_side', side=[dx, dy], target=target)
            if label and any(norm(label) == norm(t) for t in target):
                game_input('key TAB')
                time.sleep(1.0)
                return True
            if not label:
                # no known target label (a quest NPC whose on-screen name is unrecorded): the teleport faced
                # them, so TAB acts on them; the quest's own state tells whether the conversation started
                log('talk_blind', side=[dx, dy], target=target)
                game_input('key TAB')
                time.sleep(1.0)
                return True
        return False

    if q.get('start'):
        s = q['start']
        active, _ = ev(f"tostring(quest:IsQuestActive('{s['quest']}'))", 20)
        if active != 'true' and s.get('card'):
            if not take_card(s):
                log('start_failed', quest=s['quest'])
                return 'failed'
            log('card_taken', quest=s['quest'])
        # assistance statements run once in the host BEFORE travel (Bandit Camp: the disguise the gate guard checks,
        # given and worn through GiveHeroObject / SetHeroAsWearing instead of the inventory UI, must be on before any
        # bandit sees the hero). An item-received box pauses the game until it is clicked away, so each statement is
        # followed by clearing boxes; a statement sent while one is up can be lost (SetHeroAsWearing in run bc2).
        for line in s.get('setup', []):
            reply = send(f'{host}: {line}', 20)
            for _ in range(4):
                if game_input('clear').strip() == '(no box)':
                    break
                time.sleep(0.5)
            log('setup', line=line, reply=[r for r in reply if 'error' in r.lower() or ' ok ' in r][:2])
        if s.get('setup') and s.get('setupCheck'):
            val, _ = ev(s['setupCheck'], 20)
            log('setup_check', value=val)
            if val != 'true':
                log('start_failed', quest=s['quest'], reason='setup check failed')
                return 'failed'
        dest = s.get('travel')
        last = (dest[-1] if dest and isinstance(dest[0], list) else dest) or []
        arrived = active == 'true' and len(last) > 4 and ev(f"tostring(quest:IsRegionLoaded('{last[4]}'))", 10)[0] == 'true'
        if dest and not arrived:            # (attached to a game already there: no second crossing)
            if not travel(dest):
                log('start_failed', quest=s['quest'], reason='travel failed')
                return 'failed'
        if s.get('talk') and not talk(s['talk'], s.get('talkLabel', '')):
            log('start_failed', quest=s['quest'], talk=s['talk'])
            return 'failed'

    deadline = time.time() + a.minutes * 60
    paused, last_log, polls, fights = 0, 0, 0, 0
    # quest steps after the start: talk to a script-named NPC, then wait for `until` (Lua in the step's stateHost)
    steps, step_i, step_started, step_tries = q.get('steps', []), 0, None, 0
    step_fight = False          # a step's own fight targets are set in the Lua runner (Runner.fightNames)
    while time.time() < deadline:
        game_input('move 512 384')          # keep the window focused (the game freezes in the background)
        val, err = ev("Runner and Runner.status or 'no runner'", timeout=4)
        if val is None:
            # frames are paused: a yes/no question (answer by rule), a game-info box (clear), or a full-screen
            # menu such as Quest Start (ENTER). The answer goes first: a question is the common case mid-route and
            # it ignores input for a moment after it appears, so the ladder repeats quickly.
            paused += 1
            answer = 'hold RMB 100' if q.get('answer', 'no') == 'no' else 'hold LMB 100'
            # a quest can replace the ladder: when no question is up the mouse answer is an attack in the world
            # (bc3: a scripted conversation paused the host's frames, the RMB "no" swung at the disguised hero's gate
            # guard and his AI fought back)
            current = steps[step_i] if step_i < len(steps) else {}
            ladder = current.get('pauseSteps') or q.get('pauseSteps') or [answer, 'clear', answer, 'key ENTER']
            step = ladder[(paused - 1) % len(ladder)]
            if paused % 4 == 1:
                shot = out / f'{a.tag}_pause_{int(time.time())}.png'
                game_input(f'capture {shot}')
            r = game_input(step).strip()
            log('pause', step=step, result=r[:60])
            continue
        paused = 0
        st = parse_status(val)
        if val.startswith('ERROR') or err:
            log('runner_error', status=val, err=err)
        if time.time() - last_log > 5:
            log('status', status=val)
            last_log = time.time()
        if q.get('party'):
            # the party size the quest expects, from the quest's own state (a trader who dies is not waited for)
            n = send(f"eval {q.get('partyHost', host)}: tostring({q['party']})", 6)
            n = next((r.rsplit(' = ', 1)[-1] for r in n if '[Autopilot] eval ' in r), None)
            if n and n.isdigit():
                send(f'{host}: Runner.minParty = {n}', 6)
        done, _ = ev(f"tostring({q['done']})") if q.get('done') else (None, None)
        failed, _ = ev(f"tostring({q['failed']})") if q.get('failed') else (None, None)
        if done == 'true' or failed == 'true':
            events, _ = ev("table.concat(Runner.log, ' | ')")
            log('finished', done=done, failed=failed, events=events)
            game_input(f"capture {out / (a.tag + '_end.png')}")
            result = 'done' if done == 'true' else 'failed'
            break
        if step_i < len(steps):
            stp = steps[step_i]
            host_s = stp.get('stateHost', q.get('stateHost', host))
            if step_started is None:
                step_started, step_tries = time.time(), step_tries + 1
                log('step_start', step=stp.get('name', step_i), attempt=step_tries)
                if stp.get('travel') and not travel(stp['travel']):
                    log('step_travel_failed', step=stp.get('name', step_i))
                if stp.get('fight'):
                    names = ', '.join(repr(n) for n in stp['fight'])
                    send(f'{host}: Runner.fightNames = {{{names}}}', 10)
                    step_fight = True
                if stp.get('talk') and not talk(stp['talk'], stp.get('talkLabel', '')):
                    log('step_talk_failed', step=stp.get('name', step_i))
            reply = send(f"eval {host_s}: tostring({stp['until']})", 6)
            val = next((r.rsplit(' = ', 1)[-1] for r in reply if '[Autopilot] eval ' in r), None)
            if val == 'true':
                log('step_done', step=stp.get('name', step_i), seconds=round(time.time() - step_started))
                if stp.get('fight'):
                    names = ', '.join(repr(n) for n in q.get('fightNames', []))
                    send(f'{host}: Runner.fightNames = {{{names}}}', 10)
                    step_fight = False
                step_i, step_started, step_tries = step_i + 1, None, 0
            elif time.time() - step_started > stp.get('timeout', 180):
                if step_tries >= stp.get('tries', 2):
                    log('step_failed', step=stp.get('name', step_i), until=val)
                    result = 'failed'
                    break
                step_started = None             # talk again
        # conversations keep frames running and wait for 'Next' (the Darkwood4 camp-trader greeting held the
        # party for minutes): `clear` clicks only when a box / subtitle icon is actually on screen
        polls += 1
        if q.get('stateStatus') and polls % 3 == 1:
            log('quest_status', status=state_eval(q['stateStatus'], 6))
        if st.get('party') == 'short' or polls % 3 == 0:
            r = game_input('clear').strip()
            if r != '(no box)':
                log('clicked_next', result=r[:40])
        if 'pos' not in st or st.get('scene') == 'true':
            time.sleep(1)
            continue
        if q.get('fightNames') or q.get('fightDefs') or step_fight:
            # a death a script must see as a kill BY the hero (MsgIsKilledBy): the runner drains the target to 1
            # health and places the hero beside it; the last blow is a real attack (sword, then a spell)
            ready = not q.get('fightWhen') or state_eval(f"tostring({q['fightWhen']})", 6) == 'true'
            fight, _ = ev('Runner.fightStep(quest)', 6) if ready else (None, None)
            if fight and fight != 'none' and not fight.startswith('ERROR'):
                fights += 1
                if fights == 1 or fights % 10 == 0:
                    game_input('key Q')             # draw the melee weapon
                inputs = q.get('fightInputs', {})
                sequence = inputs.get(fight.split()[0], inputs.get('default'))
                for step in sequence or ['chord LSHIFT 1200' if fights % 4 == 0 else 'lmb 4']:
                    game_input(step)
                if fights % 5 == 1:
                    log('fight', target=fight, step=fights)
                continue
        region = st.get('region') or world.get(st.get('map'), '?')
        leg = q['legs'].get(region)
        if q.get('hunt') and st.get('route') == '0':
            # a fight the quest waits on: walk to the nearest living enemy the quest names (clearing kills it
            # once it is within reach); the region's leg resumes when none is left
            near, _ = ev(HUNT_EXPR, 6)
            if near and re.fullmatch(r'-?[\d.]+,-?[\d.]+,-?[\d.]+', near):
                ex, ey, dist = (float(t) for t in near.split(','))
                if dist > 8:
                    leg = [ex, ey]
        if leg and st.get('route') == '0':
            x, y, z = st['pos']
            pts = grid.waypoints((x, y), tuple(leg[:2]))
            if not pts:
                log('no_path', region=region, pos=[x, y], to=leg)
                time.sleep(3)
                continue
            if len(leg) > 2:
                pts[-1] = (leg[0], leg[1], leg[2])
            # the last waypoint is the region exit when the leg gives its z (the runner retries an exit that did not fire)
            # [x, y, z, false]: the exact z of a spot that is NOT an exit (Barrow Fields' end marker is on a bridge,
            # the terrain z under it left the hero 4.5 units off a 3-unit trigger, te8)
            is_exit = len(leg) > 2 and (len(leg) < 4 or bool(leg[3]))
            route = ', '.join(f'{{x={p[0]:.2f},y={p[1]:.2f},z={p[2]:.2f}{",exit=true" if is_exit and k == len(pts) - 1 else ""}}}'
                              for k, p in enumerate(pts))
            send(f'{host}: if Runner.region == "{region}" then Runner.route = {{{route}}}; Runner.routeRegion = "{region}" end')
            log('route', region=region, hops=len(pts), to=leg)
        time.sleep(0.5)
    else:
        log('timeout', minutes=a.minutes)
        result = 'timeout'
    events, _ = ev("table.concat(Runner.log, ' | ')")
    log('events', events=events)
    send(f'{host}: Runner.running = false; Runner.route = {{}}')
    return result


if __name__ == '__main__':
    main()
