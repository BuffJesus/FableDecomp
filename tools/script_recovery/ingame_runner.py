"""Hands-free playtest driver: installs tools/script_recovery/ingame_runner.lua (per-frame reflexes: health,
hostile clearing, follower-paced route hops) as a thread of an always-alive quest host, then plans and
watches from here without a model in the loop.

    python tools/script_recovery/ingame_runner.py tools/script_recovery/runner_quests/trader_escort.json \
        --bundle v15 --tag te1 [--minutes 30]

Quest file (JSON):
  host       quest VM the runner lives in (LUAGameflow is alive for the whole game)
  maps       regex over map / region names whose LEVs the planner loads
  legs       {region: [x, y] | [x, y, z]} -- where to walk in each region (usually its onward region exit)
  enemyNames script names the runner clears even when the area query misses them
  answer     "yes" | "no": the default answer to a yes/no question (frames pause while it is up)
  done       Lua expression, evaluated in `host`, true when the test succeeded
  failed     Lua expression, true when it failed
Log: work/runner/<tag>.jsonl (one status per poll) + screenshots at stalls and pauses.
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
from tools.script_recovery.autopilot import Channel, game_input  # noqa: E402
from tools.script_recovery.walkgrid import Grid  # noqa: E402

RUNNER_LUA = Path(__file__).with_name('ingame_runner.lua')
# things the planner routes around everywhere (TNG definitions): Darkwood's exploding spores
HAZARDS = {'OBJECT_EXPLODING_SPORE_MEDIUM', 'OBJECT_EXPLODING_SPORE_LARGE'}


def parse_status(s):
    out = {}
    for k, v in re.findall(r'(\w+)=(\S+)', s or ''):
        out[k] = v
    if 'pos' in out:
        out['pos'] = tuple(float(t) for t in out['pos'].split(','))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('quest', type=Path)
    ap.add_argument('--bundle', default='v15')
    ap.add_argument('--tag', default='run')
    ap.add_argument('--minutes', type=float, default=30)
    a = ap.parse_args()
    q = json.loads(a.quest.read_text(encoding='utf-8'))
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

    def log(kind, **kw):
        logf.write(json.dumps({'t': round(time.time(), 1), 'kind': kind, **kw}) + '\n')
        logf.flush()
        print(kind, json.dumps(kw)[:220], flush=True)

    # install: the file loads with the HOST environment as _ENV (the chunk's own env falls back to it)
    path = str(RUNNER_LUA).replace('\\', '/')
    install = (f"{host}: local E = (getmetatable(_ENV) or {{}}).__index or _ENV; local f = assert(loadfile('{path}', 't', E)); f(); "
               f"E.Runner.enemyNames = {{{', '.join(repr(n) for n in q.get('enemyNames', []))}}}; "
               f"E.Runner.partyNames = {{{', '.join(repr(n) for n in q.get('partyNames', []))}}}; "
               f"if not E.Runner.thread then E.Runner.thread = true; quest:CreateThread('RunnerMain') end")
    log('install', reply=[r for r in send(install, 20) if 'error' in r.lower()][:2])
    regions = ', '.join(f'["{m}"]="{r}"' for m, r in world.items() if m in grid.maps and re.search(q['maps'], m + ' ' + r))
    send(f'{host}: Runner.regionOf = {{{regions}}}', 20)

    deadline = time.time() + a.minutes * 60
    paused, last_log, polls = 0, 0, 0
    while time.time() < deadline:
        game_input('move 512 384')          # keep the window focused (the game freezes in the background)
        val, err = ev("Runner and Runner.status or 'no runner'", timeout=4)
        if val is None:
            # frames are paused: a yes/no question (answer by rule), a game-info box (clear), or a full-screen
            # menu such as Quest Start (ENTER). The answer goes first: a question is the common case mid-route and
            # it ignores input for a moment after it appears, so the ladder repeats quickly.
            paused += 1
            answer = 'hold RMB 100' if q.get('answer', 'no') == 'no' else 'hold LMB 100'
            step = [answer, 'clear', answer, 'key ENTER'][(paused - 1) % 4]
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
            break
        # conversations keep frames running and wait for 'Next' (the Darkwood4 camp-trader greeting held the
        # party for minutes): `clear` clicks only when a box / subtitle icon is actually on screen
        polls += 1
        if st.get('party') == 'short' or polls % 3 == 0:
            r = game_input('clear').strip()
            if r != '(no box)':
                log('clicked_next', result=r[:40])
        if 'pos' not in st or st.get('scene') == 'true':
            time.sleep(1)
            continue
        region = st.get('region') or world.get(st.get('map'), '?')
        leg = q['legs'].get(region)
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
            route = ', '.join(f'{{x={p[0]:.2f},y={p[1]:.2f},z={p[2]:.2f}{",exit=true" if len(leg) > 2 and k == len(pts) - 1 else ""}}}'
                              for k, p in enumerate(pts))
            send(f'{host}: if Runner.region == "{region}" then Runner.route = {{{route}}}; Runner.routeRegion = "{region}" end')
            log('route', region=region, hops=len(pts), to=leg)
        time.sleep(0.5)
    else:
        log('timeout', minutes=a.minutes)
    events, _ = ev("table.concat(Runner.log, ' | ')")
    log('events', events=events)
    send(f'{host}: Runner.running = false; Runner.route = {{}}')


if __name__ == '__main__':
    main()
