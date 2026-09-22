"""Autopilot driver for the NoviCompatibility exec channel (docs/scripts/AUTOPILOT_DESIGN.md).

The sidecar polls `<bundle>/NoviCompatibility/autopilot/commands.txt` (see FableScriptExtender/Autopilot.h in
the sidecar tree) and answers on the FSE log with `[Autopilot] ...` lines. This driver writes command batches,
waits for the batch's `processed` marker, and runs checklists of steps against the log.

    python tools/script_recovery/autopilot.py send v6 "list"
    python tools/script_recovery/autopilot.py send v6 "dump Q_GuildTrainingPreMelee"
    python tools/script_recovery/autopilot.py send v6 "Q_GuildTrainingPreMelee: quest:SetStateInt('DummyHits', 7)"
    python tools/script_recovery/autopilot.py run v6 tools/script_recovery/checklists/guild_woods_return.json
    python tools/script_recovery/autopilot.py tail v6          # print [Autopilot] lines as they arrive

A checklist is a JSON list of steps:
    {"id": "punch", "do": ["Q_GuildTrainingPreMelee: quest:SetStateInt('DummyHits', 7)"],
     "expect": "TEXT_CS_028_PREMELEE_STICK", "forbid": "LUA RUNTIME ERROR", "timeout": 30}
`do` lines go to the channel; `input: key <KEYS>` / `input: click <X> <Y>` / `input: capture <file>` lines drive the
game window through the FableForge frontend driver (gamewin.ps1: real keyboard/mouse input); `expect` is a regex the log must show after the step's batch marker within `timeout`
seconds; `forbid` is a regex that fails the step if it appears in the same window. The run stops at the first
failure, dumps every live quest, and archives the log through ab_playtest.collect.

`run --launch --save <profile>` does the front half hands-free: stages that profile's AutoSave into the folder the
`0atlas` frontend row loads (1234234; originals backed up and restored afterwards), launches the bundle through
ab_playtest (which archives the log when the game exits), clicks title -> Change Profile -> 0atlas -> Continue ->
AutoSave, and waits for the first quest host before the checklist starts. Without `--launch` the driver only talks
to a game that is already running with the bundle. Never launch Fable while another session needs the game.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
WORK = ROOT / 'work' / 'new-oakvale-original-fse-20260912'
GAMEWIN = Path(__file__).with_name('gamewin.ps1')   # window driver (real input); copy of FableForge tools/ingame/gamewin.ps1 + lmb/hold/scancodes


def game_input(spec: str, timeout: float = 60.0) -> str:
    """`key ENTER ESC` / `click 660 398` / `capture <png>` through gamewin.ps1; returns its stdout."""
    import subprocess
    parts = spec.split()
    if not parts:
        return ''
    if parts[0] == 'key':
        args = ['-Action', 'key', '-Keys', ' '.join(parts[1:])]
    elif parts[0] in ('click', 'move') and len(parts) == 3:
        args = ['-Action', parts[0], '-X', parts[1], '-Y', parts[2]]
    elif parts[0] == 'capture' and len(parts) == 2:
        args = ['-Action', 'capture', '-Output', parts[1]]
    elif parts[0] == 'hold' and len(parts) == 3:       # `hold W 3000`: hold a key for N ms (walk)
        args = ['-Action', 'hold', '-Keys', parts[1], '-X', parts[2]]
    elif parts[0] == 'chord' and len(parts) == 3:      # `chord LSHIFT 1200`: hold a modifier and, inside it, the left button for N ms (cast a spell)
        args = ['-Action', 'chord', '-Keys', parts[1], '-X', parts[2]]
    elif parts[0] == 'clear':                     # click ONLY when a game-info box / question is up: a click with a weapon
        png = ROOT / 'scratchpad' / 'autopilot_clear.png'   #   drawn and no box is an attack (three friendly hits = the
        game_input(f'capture {png}', timeout)                #   Guild's third warning, which ended the Skill stage, 2026-09-20)
        pos = game_info_box_pos(png)
        #   click ON the box's mouse icon / 'Next' label: a bare `lmb` where the cursor happens to sit drew the bow
        #   instead of dismissing the Skill stage's second instruction box (run 9b, 2026-09-21)
        return game_input(f'click {pos[0] + 20} {pos[1]}', timeout) if pos else '(no box)'
    elif parts[0] == 'skip':                      # ESC only when the pause menu is NOT up (ESC outside a scene opens it);
        png = ROOT / 'scratchpad' / 'autopilot_skip.png'   #   with the menu up, ESC closes it instead
        game_input(f'capture {png}', timeout)
        kind = screen_kind(png)
        return game_input('key ESC', timeout) if kind != 'pausemenu' else game_input('key ESC', timeout) + ' (closed the pause menu)'
    elif parts[0] == 'focus':                     # bring the game window to the foreground (Fable freezes cutscene
        args = ['-Action', 'info']                #   timers while it is not the foreground window)
    elif parts[0] == 'lmb':                       # `lmb [count]`: attack / confirm clicks where the cursor is
        args = ['-Action', 'lmb', '-X', parts[1] if len(parts) > 1 else '1']
    else:
        raise ValueError(f'bad input spec {spec!r} (key <KEYS> | hold <KEY> <ms> | chord <KEY> <ms> | click <X> <Y> | move <X> <Y> | lmb [count] | clear | skip | focus | capture <file>)')
    r = subprocess.run(['powershell', '-NoProfile', '-File', str(GAMEWIN)] + args, capture_output=True, text=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(f'gamewin {spec!r} failed: {r.stderr.strip()[-300:]}')
    return r.stdout


SAVES = Path.home() / 'Documents' / 'My Games' / 'Fable' / 'Saves'
FRONTEND_PROFILE_DIR = '1234234'        # what the frontend's '0atlas' row actually loads (its Profile.bin names this folder)


def stage_save(profile: str) -> Path | None:
    """copy <profile>/AutoSave* over the 0atlas-loaded folder; returns the backup dir (restore with restore_save)"""
    import shutil
    src, dst = SAVES / profile, SAVES / FRONTEND_PROFILE_DIR
    if not (src / 'AutoSave').is_file():
        sys.exit(f'no AutoSave in profile {src}')
    backup = ROOT / 'scratchpad' / f'save_backup_{FRONTEND_PROFILE_DIR}'
    backup.mkdir(parents=True, exist_ok=True)
    for f in ('AutoSave', 'AutoSave.qs', 'AutoSave.qs.hs'):
        if (dst / f).is_file():
            shutil.copy2(dst / f, backup / f)
        shutil.copy2(src / f, dst / f)
    print(f'staged {profile}/AutoSave -> {dst} (backup {backup})')
    return backup


    """Copy the live AutoSave (what the run left behind) into SAVES/<profile>, creating it."""
    import shutil
    live, dst = SAVES / FRONTEND_PROFILE_DIR, SAVES / profile
    if not (live / 'AutoSave').is_file():
        print(f'harvest: no AutoSave in {live}; nothing to keep')
        return
    dst.mkdir(parents=True, exist_ok=True)
    for f in ('AutoSave', 'AutoSave.qs', 'AutoSave.qs.hs'):
        if (live / f).is_file():
            shutil.copy2(live / f, dst / f)
    if not (dst / 'Profile.bin').is_file():
        src_profile = SAVES / source if source else None
        if src_profile and (src_profile / 'Profile.bin').is_file():
            shutil.copy2(src_profile / 'Profile.bin', dst / 'Profile.bin')
    print(f'harvested the run\'s AutoSave -> {dst}')


def restore_save(backup: Path | None) -> None:
    import shutil
    if not backup:
        return
    dst = SAVES / FRONTEND_PROFILE_DIR
    for f in ('AutoSave', 'AutoSave.qs', 'AutoSave.qs.hs'):
        if (backup / f).is_file():
            shutil.copy2(backup / f, dst / f)
    print(f'restored {dst} from {backup}')


def screen_kind(png: Path) -> str:
    """classify a frontend capture by region brightness/colour (measured on 2026-09-20 captures, 1024x768):
    title / menu / profiles / load / other (in-game, quest screens, boxes)"""
    from PIL import Image
    import numpy as np
    im = np.asarray(Image.open(png).convert('RGB')).astype(float)

    def region(x0, y0, x1, y1):
        return im[y0:y1, x0:x1].reshape(-1, 3)
    grey = lambda a: a.mean(1)  # noqa: E731
    bright = lambda a: (grey(a) > 200).mean()  # noqa: E731
    pills = region(12, 128, 190, 292)                          # the in-game pause menu's grey button column (top-left)
    if float(((grey(pills) > 150) & (np.abs(pills[:, 0] - pills[:, 2]) < 18)).mean()) > 0.05:
        return 'pausemenu'
    header = bright(region(100, 76, 270, 98))                  # "Select Profile" / "<profile> - Load Game" header text
    if header > 0.10 and bright(region(440, 198, 580, 220)) > 0.15:
        return 'profiles'                                      # "New Profile" row
    if header > 0.10 and bright(region(150, 150, 270, 172)) > 0.15:
        return 'load'                                          # "AutoSave" row
    if grey(region(20, 680, 400, 715)).mean() < 30:            # no Back/Delete bar: title or main menu
        if bright(region(380, 325, 650, 350)) > 0.15:
            return 'menu'                                      # row 1 "<profile> - Continue Game"
        if bright(region(280, 388, 740, 412)) > 0.10:
            return 'title'                                     # "Press Left Mouse Button To Continue"
    return 'other'


def load_header_is(png: Path, reference: Path) -> bool:
    """the '<profile> - Load Game' header text region matches the reference capture (pixel diff)"""
    from PIL import Image
    import numpy as np
    a = np.asarray(Image.open(png).convert('L')).astype(float)[75:100, 30:310]
    b = np.asarray(Image.open(reference).convert('L')).astype(float)[75:100, 30:310]
    return float(np.abs(a - b).mean()) < 12.0


PROFILE_ROWS = [209, 254, 298, 343, 388, 433, 478, 523, 567]     # y of the nine visible Select Profile rows


def highlighted_profile_row(png: Path) -> int:
    """y of the row the light-blue hover bar is on (blue tint of the bar left/right of the text column)"""
    from PIL import Image
    import numpy as np
    im = np.asarray(Image.open(png).convert('RGB')).astype(float)

    def score(y):
        a = np.concatenate([im[y - 10:y + 10, 300:420].reshape(-1, 3), im[y - 10:y + 10, 600:720].reshape(-1, 3)])
        return float(((a[:, 2] - a[:, 0]) * (a.mean(1) > 140)).mean())
    return PROFILE_ROWS[int(np.argmax([score(y) for y in PROFILE_ROWS]))]


def hover_profile_row(target_y: int, shots: Path, tag: str) -> bool:
    """move the cursor until the hover bar sits on the wanted row (the DirectInput cursor walk lands ~1 row off)"""
    y = target_y
    for attempt in range(4):
        game_input(f'move 512 {y}')
        time.sleep(1.0)
        png = shots / f'{tag}_hover{attempt}.png'
        game_input(f'capture {png}')
        got = highlighted_profile_row(png)
        if got == target_y:
            return True
        y += target_y - got
    return False


def game_info_box_up(png: Path) -> bool:
    """a game-info / tutorial / item box or a YES-NO question is on screen: the bright saturated-green mouse
    icon next to its 'Next' / answer labels (measured on the 2026-09-20 captures: >=20 such pixels in the
    lower-right quarter, in-world scenes 0-4; rows to 720: a Speak's subtitle box puts its 'Next' icon at y~687, run 12)"""
    from PIL import Image
    import numpy as np
    im = np.asarray(Image.open(png).convert('RGB')).astype(float)
    a = im[370:720, 560:960].reshape(-1, 3)
    return int(((a[:, 1] > 170) & (a[:, 1] - a[:, 0] > 60) & (a[:, 1] - a[:, 2] > 60)).sum()) >= 20


def game_info_box_pos(png: Path) -> tuple[int, int] | None:
    """window position of the box's green mouse icon (the centroid of its pixels), None when no box is up"""
    from PIL import Image
    import numpy as np
    im = np.asarray(Image.open(png).convert('RGB')).astype(float)
    a = im[370:720, 560:960]
    mask = (a[:, :, 1] > 170) & (a[:, :, 1] - a[:, :, 0] > 60) & (a[:, :, 1] - a[:, :, 2] > 60)
    if int(mask.sum()) < 20:
        return None
    ys, xs = np.nonzero(mask)
    # the icon sits BELOW the (green) instruction text, on the 'Next' row: the lowest 18 px band of green pixels
    sel = ys >= ys.max() - 18
    return int(xs[sel].mean()) + 560, int(ys[sel].mean()) + 370


def drive_frontend_to_autosave(shots: Path, timeout: float = 120.0) -> None:
    """title -> (menu -> Change Profile) -> 0atlas -> Continue Game -> AutoSave, verifying every screen by capture;
    the main menu opens on the last profile, or on the profile list when the game has none (after a crash)"""
    shots.mkdir(parents=True, exist_ok=True)
    profile_selected = False
    deadline = time.time() + timeout
    step = 0
    while time.time() < deadline:
        step += 1
        png = shots / f'fe_{step:02d}.png'
        game_input(f'capture {png}')
        kind = screen_kind(png)
        print(f'frontend: {kind}')
        if kind == 'title':
            game_input('click 512 400')
        elif kind == 'menu':
            game_input('click 512 337' if profile_selected else 'click 512 385')   # Continue Game / Change Profile
        elif kind == 'profiles':
            # the list ignores the arrow keys and the DirectInput cursor walk lands about one row (44 px) off:
            # hover, verify the highlight bar by capture, correct, then click. Rows: New Profile, 0aa, 0atlas.
            if hover_profile_row(298, shots, f'fe_{step:02d}'):
                game_input('lmb 1')
                profile_selected = True
            else:
                print('frontend: could not put the hover bar on 0atlas')
        elif kind == 'load':
            if not load_header_is(png, Path(__file__).parent / 'testdata' / 'frontend' / 'load_1234234.png'):
                print('frontend: Load Game screen for the wrong profile; back to the profile list')
                game_input('key ESC')
                profile_selected = False
                time.sleep(4)
                continue
            game_input('click 207 161')                                            # AutoSave
            return
        else:
            time.sleep(3)
            continue
        time.sleep(4)
    sys.exit('frontend: could not reach the Load Game screen')


def launch_and_load(bundle: str, timeout: float = 240.0) -> None:
    """launch the bundle (ab_playtest, background), then drive the frontend to the 0atlas AutoSave and wait for a host"""
    import subprocess
    log = log_path(bundle)
    if log.is_file():
        log.unlink()                                 # a fresh log: the launcher appends, and stale markers would satisfy waits
    proc = subprocess.Popen([sys.executable, str(ROOT / 'tools' / 'script_recovery' / 'ab_playtest.py'), 'launch', bundle],
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)   # archives the log when Fable exits
    r = subprocess.run(['powershell', '-NoProfile', '-File', str(GAMEWIN), '-Action', 'wait', '-Seconds', '120'], capture_output=True, text=True)
    if r.returncode != 0:
        proc.poll()
        sys.exit(f'no Fable window: {r.stdout.strip()}')
    deadline = time.time() + timeout
    while time.time() < deadline and not (log.is_file() and 'hosts enabled' in log.read_text(encoding='utf-8', errors='replace')):
        time.sleep(2)
    time.sleep(14)                                   # title screen
    tail = LogTail(log)
    drive_frontend_to_autosave(ROOT / 'scratchpad' / 'autopilot_frontend')
    deadline = time.time() + timeout
    while time.time() < deadline:
        if any('LuaQuestHost for ' in l and 'created' in l for l in tail.new_lines()):
            print('save loaded: first quest host is up')
            time.sleep(10)                           # the loading screen / opening cutscene: script frames start soon after
            return
        time.sleep(2)
    sys.exit('the save did not load (no quest host within the timeout)')


def bundle_dir(name: str) -> Path:
    d = WORK / f'local-candidate-{name}'
    if not d.is_dir():
        sys.exit(f'no bundle {d}')
    return d


def log_path(name: str) -> Path:
    return bundle_dir(name) / 'NoviCompatibility' / 'FableScriptExtender.log'


def commands_path(name: str) -> Path:
    return bundle_dir(name) / 'NoviCompatibility' / 'autopilot' / 'commands.txt'


class LogTail:
    """Read new COMPLETE lines of the FSE log since the last call (the log is append-only while the game runs).

    Binary reads at a byte offset; a trailing partial line (the DLL writes line then endl, we may read in between)
    is held back until its newline arrives, so a marker is never split across two calls.
    """

    def __init__(self, path: Path, back_lines: int = 0):
        self.path = path
        self.pos = path.stat().st_size if path.is_file() else 0
        self.partial = b''
        if back_lines and self.pos:
            # start N lines earlier: a run ATTACHED to a live game (no --launch) whose first marker was logged just
            # before the attach (the WoodsMelee host line beat the resumed driver by seconds, 2026-09-21)
            data = path.read_bytes()
            cut = len(data)
            for _ in range(back_lines):
                nl = data.rfind(b'\n', 0, max(cut - 1, 0))
                if nl < 0:
                    cut = 0
                    break
                cut = nl + 1
            self.pos = cut

    def new_lines(self) -> list[str]:
        if not self.path.is_file():
            return []
        with self.path.open('rb') as f:
            f.seek(self.pos)
            data = f.read()
            self.pos = f.tell()
        data = self.partial + data
        if not data:
            return []
        if data.endswith(b'\n'):
            self.partial = b''
        else:
            data, _, self.partial = data.rpartition(b'\n')
            if not data and not self.partial:
                return []
        return data.decode('utf-8', errors='replace').splitlines()


class Channel:
    def __init__(self, bundle: str):
        self.bundle = bundle
        self.log = log_path(bundle)
        self.commands = commands_path(bundle)
        self.batch = 0

    def send(self, lines: list[str], timeout: float = 10.0) -> list[str]:
        """Write one batch and return the `[Autopilot]` lines it produced (waits for the `processed` marker)."""
        self.batch += 1
        tail = LogTail(self.log)
        self.commands.parent.mkdir(parents=True, exist_ok=True)
        tmp = self.commands.with_suffix('.tmp')
        tmp.write_text(f'batch {self.batch}\n' + '\n'.join(lines) + '\n', encoding='utf-8')
        tmp.replace(self.commands)            # atomic: the poller never sees a half-written file
        deadline = time.time() + timeout
        got: list[str] = []
        while time.time() < deadline:
            for line in tail.new_lines():
                if '[Autopilot]' in line:
                    got.append(line[line.index('[Autopilot]'):])
                    if line.rstrip().endswith(' line(s)') and 'processed' in line:
                        return got
            time.sleep(0.1)
        got.append(f'[Autopilot] TIMEOUT after {timeout}s (is the game running with this bundle? is a script advancing frames?)')
        return got


def run_checklist(bundle: str, steps: list[dict], default_timeout: float = 30.0, tail_back: int = 0) -> list[dict]:
    ch = Channel(bundle)
    results = []
    # one tail for the whole checklist: a marker logged between two steps (PASSED_20 three seconds after PASSED_10,
    # while the driver was still finishing the previous step) must count for the next step
    tail = LogTail(ch.log, tail_back)
    carry = ''          # log text read past the previous step's marker (the same read chunk), owed to the next step
    for step in steps:
        sid = step.get('id', f'step{len(results) + 1}')
        do = step.get('do', [])
        expect = step.get('expect')
        forbid = step.get('forbid')
        timeout = float(step.get('timeout', default_timeout))
        inputs = [d[6:].strip() for d in do if isinstance(d, str) and d.startswith('input:')]
        commands = [d for d in do if isinstance(d, str) and not d.startswith('input:')]
        outcome = {'id': sid, 'status': 'PASS', 'detail': ''}
        if inputs and not GAMEWIN.is_file():
            outcome.update(status='SKIPPED', detail=f'input steps need {GAMEWIN}: ' + '; '.join(inputs))
            results.append(outcome)
            print(f'{sid}: SKIPPED ({outcome["detail"]})')
            continue

        repeats = int(step.get('repeat', 1))
        interval = float(step.get('interval', 2.0))

        def act() -> list[str]:
            """one round of the step's actions, in `do` order: runs of channel lines go as one batch, `input:`
            lines drive the window in between (a game-info box pauses script frames, so its click must come first)"""
            got: list[str] = []
            pending: list[str] = []

            def flush() -> None:
                if pending:
                    got.extend(ch.send(pending, timeout=min(timeout, 15.0 if repeats == 1 else 5.0)))
                    pending.clear()
            for d in do:
                if not isinstance(d, str):
                    continue
                if d.startswith('input:'):
                    flush()
                    try:
                        game_input(d[6:].strip())
                    except Exception as e:  # noqa: BLE001 - reported as a step failure
                        got.append(f'[Autopilot] error input {d[6:].strip()}: {e}')
                else:
                    pending.append(d)
            flush()
            return got
        # `repeat`: re-send the commands every `interval` seconds until the marker shows (a respawning spawner)
        if GAMEWIN.is_file():
            try:
                game_input('focus')          # a background Fable window stalls cutscene waits (5-minute BOOHOO hang, 2026-09-20)
            except Exception as e:  # noqa: BLE001
                print(f'{sid}: focus failed: {e}')
        replies = act()
        errors = [r for r in replies if 'error' in r or 'exception' in r or 'TIMEOUT' in r]
        # a channel TIMEOUT on a repeated step is not final: a game-info box pauses script frames until the
        # step's own click clears it, so keep re-sending until the repeat budget is spent
        if errors and repeats > 1 and all('TIMEOUT' in e for e in errors):
            errors = []
        if errors:
            outcome.update(status='FAIL', detail='channel: ' + ' | '.join(errors))
        elif expect:
            deadline = time.time() + timeout
            seen, carry = carry, ''
            sent, next_send = 1, time.time() + interval
            next_focus = time.time() + 15.0
            while time.time() < deadline and outcome['status'] == 'PASS':
                if GAMEWIN.is_file() and time.time() >= next_focus:
                    try:
                        game_input('focus')
                    except Exception:  # noqa: BLE001
                        pass
                    next_focus = time.time() + 15.0
                chunk = tail.new_lines()
                seen += '\n'.join(chunk) + '\n'
                if forbid and re.search(forbid, seen):
                    outcome.update(status='FAIL', detail=f'forbidden marker /{forbid}/ appeared')
                    break
                m = re.search(expect, seen)
                if m:
                    outcome['detail'] = f'expected /{expect}/ seen' + (f' after {sent} send(s)' if repeats > 1 else '')
                    carry = seen[m.end():]
                    break
                if (commands or inputs) and sent < repeats and time.time() >= next_send:
                    act()
                    sent += 1
                    next_send = time.time() + interval
                time.sleep(0.2)
            else:
                if outcome['status'] == 'PASS':
                    outcome.update(status='FAIL', detail=f'expected /{expect}/ not seen within {timeout}s')
        results.append(outcome)
        print(f'{sid}: {outcome["status"]} {outcome["detail"]}')
        if outcome['status'] == 'FAIL':
            for r in ch.send(['list'], timeout=5):
                if r.startswith('[Autopilot] host '):
                    ch.send(['dump ' + r.split('host ', 1)[1].strip()], timeout=5)
            break
    return results


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    s = sub.add_parser('send'); s.add_argument('bundle'); s.add_argument('lines', nargs='+'); s.add_argument('--timeout', type=float, default=10)
    r = sub.add_parser('run'); r.add_argument('bundle'); r.add_argument('checklist', nargs='+', help='one or more checklist JSON files, run in order'); r.add_argument('--report')
    r.add_argument('--tail-back', type=int, default=0, help='attached runs: start the log tail N lines before the end (a marker logged just before the attach)')
    r.add_argument('--launch', action='store_true', help='launch the bundle and load the 0atlas AutoSave first')
    r.add_argument('--save', help='profile whose AutoSave to stage into the 0atlas-loaded folder before launching (restored afterwards)')
    r.add_argument('--harvest-save', metavar='PROFILE', help='after the run, copy the AutoSave it left behind into this profile (the graduation autosave is the adult save the post-Guild checklists need)')
    t = sub.add_parser('tail'); t.add_argument('bundle')
    a = ap.parse_args()
    if a.cmd == 'send':
        for line in Channel(a.bundle).send(a.lines, timeout=a.timeout):
            print(line)
    elif a.cmd == 'run':
        steps = [st for path in a.checklist for st in json.loads(Path(path).read_text(encoding='utf-8'))]
        backup = stage_save(a.save) if a.save else None
        try:
            if a.launch:
                launch_and_load(a.bundle)
            results = run_checklist(a.bundle, steps, tail_back=0 if a.launch else a.tail_back)
        finally:
            if a.harvest_save:
                harvest_save(a.harvest_save, a.save)
            restore_save(backup)
        if a.report:
            Path(a.report).write_text(json.dumps(results, indent=2), encoding='utf-8')
        failed = [x for x in results if x['status'] == 'FAIL']
        print(f'{len(results)} step(s), {len(failed)} failed')
        sys.exit(1 if failed else 0)
    elif a.cmd == 'tail':
        tail = LogTail(log_path(a.bundle))
        try:
            while True:
                for line in tail.new_lines():
                    if '[Autopilot]' in line or 'LUA RUNTIME ERROR' in line:
                        print(line.rstrip())
                time.sleep(0.25)
        except KeyboardInterrupt:
            pass


if __name__ == '__main__':
    main()
