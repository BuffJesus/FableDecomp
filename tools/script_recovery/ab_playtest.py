"""A/B playtest driver for the converter-unit bundles.

    python tools/script_recovery/ab_playtest.py launch v6      # launch local-candidate-v6 (waits for Fable to exit,
    python tools/script_recovery/ab_playtest.py launch v7      #  then archives its FSE log into ab_runs/<bundle>-<ts>/)
    python tools/script_recovery/ab_playtest.py collect v6     # archive the latest log of a bundle by hand
    python tools/script_recovery/ab_playtest.py compare v6 v7  # timeline of the newest run of each, side by side

The timeline is the same set of events extracted from both logs: Lua errors, quest/entity lifecycle, Gameflow
stage transitions, override arming, cutscene macros + skip queries, Speak keys, retail-resource refusals,
persist transfers, quest info counters/timers, game-info boxes. `compare` prints each side's timeline, then the
events present on one side only (order-insensitive), then the first divergence in order.
"""
from __future__ import annotations

import argparse
import datetime
import json
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
WORK = ROOT / 'work' / 'new-oakvale-original-fse-20260912'
GAME = Path(r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters')
SAVES = Path.home() / 'Documents' / 'My Games' / 'Fable' / 'Saves'
RUNS = ROOT / 'work' / 'ab_runs'


def bundle_dir(name: str) -> Path:
    d = WORK / f'local-candidate-{name}'
    if not d.is_dir():
        sys.exit(f'no bundle {d}')
    return d


def log_path(name: str) -> Path:
    return bundle_dir(name) / 'NoviCompatibility' / 'FableScriptExtender.log'


def fable_running() -> bool:
    out = subprocess.run(['tasklist', '/FI', 'IMAGENAME eq Fable.exe'], capture_output=True, text=True).stdout
    return 'Fable.exe' in out


def collect(name: str, session: Path | None = None) -> Path:
    src = log_path(name)
    if not src.is_file():
        sys.exit(f'no log at {src}')
    session = session or RUNS / f'{name}-{datetime.datetime.now():%Y%m%d-%H%M%S}'
    session.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, session / 'FableScriptExtender.log')
    (session / 'meta.json').write_text(json.dumps({'bundle': name, 'log': str(src), 'lines': sum(1 for _ in src.open(encoding='utf-8', errors='replace'))}, indent=2))
    print(f'archived {name} log -> {session}')
    return session


def launch(name: str, wait: bool = True) -> None:
    b = bundle_dir(name)
    if fable_running():
        sys.exit('Fable.exe is already running; close it first')
    session = RUNS / f'{name}-{datetime.datetime.now():%Y%m%d-%H%M%S}'
    session.mkdir(parents=True, exist_ok=True)
    cmd = [sys.executable, str(b / 'local_test.py'), '--game-dir', str(GAME), '--launch', '--save-dir', str(SAVES)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    (session / 'launch.txt').write_text(result.stdout + result.stderr)
    print(result.stdout.strip().splitlines()[-1] if result.stdout.strip() else result.stderr)
    if result.returncode != 0:
        sys.exit(result.returncode)
    print(f'launched {name}; session {session}')
    if not wait:
        return
    time.sleep(5)
    while fable_running():
        time.sleep(5)
    collect(name, session)


# ---- timeline -------------------------------------------------------------------------------------------------
EVENTS = [
    ('error', re.compile(r'!!! (?:LUA RUNTIME ERROR|C\+\+ EXCEPTION|ERROR)[^\n]*')),
    ('quest', re.compile(r"--- Quest '([^']+)': (C\+\+ (?:Init|Main|OnPersist)\(\) (?:phase|triggered)|Lua Main\(\) returned)")),
    ('entity', re.compile(r"(?:ENTERING|EXITED) LUA CALL for '([^']+)'")),
    ('interrupt', re.compile(r'\[Interrupted\][^\n]*')),
    ('terminating', re.compile(r'\[Terminating\][^\n]*')),
    ('lifetime', re.compile(r'\[QuestLifetime\][^\n]*')),
    ('stage', re.compile(r'(?:LUAGameflow|Gameflow)[^\n]*?(?:Transitioning into stage \d+|stage \d+[^\n]*)')),
    ('persist', re.compile(r"\[PERSIST\] Transferring (?:int|bool|string|string list|float|uint) '([^']+)'")),
    ('override', re.compile(r"Armed identity-preserving retail override for '([^']+)'")),
    ('cutscene', re.compile(r"RunCutsceneMacro_Func for '([^']+)'|RunMacro\('([^']+)'")),
    ('command', re.compile(r'\[CutsceneCommandDiag\] macro=\S+ tick=\d+ command=([^\n]+)')),
    ('skip', re.compile(r'\[CutsceneCommandDiag\] skipQueryTrue[^\n]*')),
    ('speak', re.compile(r'Key: (TEXT_[A-Z0-9_]+)')),
    ('refused', re.compile(r'\[RetailResources\] TryAcquire (?:refused|granted)[^\n]*')),
    ('gameinfo', re.compile(r'GiveHeroYesNoQuestion START|DisplayGameInfo[^\n]*')),
    ('timer', re.compile(r'(Registered|Deregistered) game timer with ID: (\d+)')),
    ('flag', re.compile(r'\[QuestThreadFlag\][^\n]*')),
    ('thing', re.compile(r"GetThingWithScriptName START: '([^']+)'")),
]
NOISY = {'thing', 'timer', 'command'}


def timeline(path: Path, noisy: bool = False) -> list[tuple[int, str, str]]:
    out = []
    for n, line in enumerate(path.open(encoding='utf-8', errors='replace'), 1):
        for kind, rx in EVENTS:
            if kind in NOISY and not noisy:
                continue
            m = rx.search(line)
            if m:
                text = next((g for g in m.groups() if g), m.group(0)) if m.groups() else m.group(0)
                out.append((n, kind, text.strip()))
                break
    return out


def newest(name: str) -> Path:
    runs = sorted(RUNS.glob(f'{name}-*'), key=lambda p: p.name)
    if not runs:
        sys.exit(f'no archived runs for {name} in {RUNS}')
    return runs[-1]


def compare(a: str, b: str, noisy: bool = False) -> None:
    ra, rb = newest(a), newest(b)
    ta, tb = timeline(ra / 'FableScriptExtender.log', noisy), timeline(rb / 'FableScriptExtender.log', noisy)
    print(f'== {a}: {ra.name} ({len(ta)} events)   vs   {b}: {rb.name} ({len(tb)} events)')
    for name, t in ((a, ta), (b, tb)):
        print(f'\n-- {name} timeline')
        for n, kind, text in t:
            print(f'  {n:6d} {kind:11s} {text[:120]}')
    ka, kb = [(k, t) for _, k, t in ta], [(k, t) for _, k, t in tb]
    only_a = sorted(set(ka) - set(kb)); only_b = sorted(set(kb) - set(ka))
    print(f'\n-- only in {a} ({len(only_a)})')
    for k, t in only_a: print(f'  {k:11s} {t[:120]}')
    print(f'\n-- only in {b} ({len(only_b)})')
    for k, t in only_b: print(f'  {k:11s} {t[:120]}')
    # first in-order divergence over the non-noisy event stream
    i = 0
    while i < min(len(ka), len(kb)) and ka[i] == kb[i]:
        i += 1
    if i < min(len(ka), len(kb)):
        print(f'\n-- first divergence at event #{i}: {a}={ka[i]}  {b}={kb[i]}')
    else:
        print(f'\n-- no in-order divergence over the first {i} events')
    errs = [(n, t) for n, k, t in ta + tb if k == 'error']
    if errs:
        print('\n-- errors (both sides)')
        for n, t in errs: print(f'  {n:6d} {t[:160]}')


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest='cmd', required=True)
    s = sub.add_parser('launch'); s.add_argument('bundle'); s.add_argument('--no-wait', action='store_true')
    s = sub.add_parser('collect'); s.add_argument('bundle')
    s = sub.add_parser('compare'); s.add_argument('a'); s.add_argument('b'); s.add_argument('--noisy', action='store_true')
    s = sub.add_parser('timeline'); s.add_argument('bundle'); s.add_argument('--noisy', action='store_true')
    a = p.parse_args()
    if a.cmd == 'launch':
        launch(a.bundle, wait=not a.no_wait)
    elif a.cmd == 'collect':
        collect(a.bundle)
    elif a.cmd == 'compare':
        compare(a.a, a.b, a.noisy)
    elif a.cmd == 'timeline':
        for n, kind, text in timeline(newest(a.bundle) / 'FableScriptExtender.log', a.noisy):
            print(f'  {n:6d} {kind:11s} {text[:140]}')


if __name__ == '__main__':
    main()
