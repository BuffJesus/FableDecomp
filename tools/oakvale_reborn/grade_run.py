#!/usr/bin/env python3
"""grade_run.py -- grade an in-game run of Oakvale Reborn from the FSE log.

    python tools/oakvale_reborn/grade_run.py <bundle>/NoviCompatibility/FableScriptExtender.log [--json out.json]
    python tools/oakvale_reborn/grade_run.py --bundle work/oakvale_reborn/bundle-v1        # same, by bundle

Reads the log once and reports every CHECKLIST.md row that is decidable from the
log (the eyes-only rows are listed as MANUAL with what to look for). The road
taken (refuse / massacre / hunted) is detected from the log so only that road's
rows are graded. Nothing here proves what was on screen; it proves what the
scripts did and that nothing threw.

Exit code 1 when any graded row fails or the gate (Lua errors / `!!!`) trips.
"""
from __future__ import annotations

import argparse
import json
import pathlib
import re
import sys

REPO = pathlib.Path(__file__).resolve().parents[2]

GATE = [
    ('gate-authority', r'NOVI_AUTHORITY', 'the sidecar took authority over Q_NewOakValeIntro', True),
    ('gate-lua-errors', r'Lua error', 'no Lua errors', False),
    ('gate-bangs', r'^.*!!! ', 'no !!! lines from the sidecar', False),
]

# (row id, regex, description, must_match) -- graded for every run
COMMON = [
    ('v1-0 cold open',    r'OVR scene: macro CS_OVR_COLDOPEN done ok=true', 'cold open macro ran and unwound', True),
    ('v1-2 spawn',        r'OVR stranger: created CREATURE_\w+ beside NOVI_BookTrader|OVR stranger: already present', 'the Stranger appeared', True),
    ('v1-3 offer macro',  r'OVR scene: macro CS_OVR_OFFER done ok=true', 'the offer macro ran and unwound', True),
    ('v1-3 answer',       r'OVR scene: lua offer-answer done ok=true', 'the answer beat ran', True),
    ('v1-3 decision',     r'OVR stranger: offer (ACCEPTED|REFUSED)', 'an answer was recorded', True),
]
REFUSE = [
    ('v1-4 refuse macro', r'OVR scene: macro CS_OVR_REFUSE done ok=true', 'the refuse scene ran (no FMV)', True),
    ('v1-4 aftermath',    r'ENTERING BLOCKING.*CS_OVR_AFTERMATH_GOOD', 'the good-road aftermath was entered', True),
]
MASSACRE = [
    ('v1-5 begin',        r'OVR massacre: begin', 'the massacre started', True),
    ('v1-5 night',        r'OVR massacre: \d+ dead, night falls', 'the massacre ended in night', True),
    ('v1-9 stall timer',  r'OVR massacre: time is up', '(info) the 180 s fallback fired', None),
    ('v1-5 bully rush',    r'OVR massacre: the Bully rushes the hero', '(info) the Bully turned on the hero at 3 kills', None),
    ('v1-5 aftermath',    r'ENTERING BLOCKING.*CS_OVR_AFTERMATH_EVIL', 'the evil-road aftermath was entered', True),
]
HUNTED = [
    ('v1-7 struck',       r'OVR stranger: STRUCK by the gift', 'the sword hit registered on him', True),
    ('v1-7 hunt',         r'OVR hunted: begin', 'the hunt started', True),
    ('v1-7 night',        r'OVR hunted: night falls', 'the hunt ended in night', True),
    ('v1-7 aftermath',    r'ENTERING BLOCKING.*CS_OVR_AFTERMATH_KILLED', 'the killed-road aftermath was entered', True),
]
MANUAL = [
    ('v1-0', 'dawn framing on the cliff camera; clock reads noon afterwards'),
    ('v1-1', 'the "Oakvale looks after its own" box after the highlighting tip'),
    ('v1-2', 'hooded body has idle/walk; comment audio + subtitle + mouth on a created creature'),
    ('v1-3', 'the sword appears in his hand at the terms; the square empties and refills'),
    ('v1-5/7', 'child draw/swing animations (S6); guards fight the child; Father/Theresa cannot be hurt'),
    ('v1-6', 'save/reload before the offer and after each answer'),
]


def grade(log: str) -> tuple[list[dict], str]:
    lines = log.splitlines()
    def hits(rx: str) -> list[str]:
        r = re.compile(rx)
        return [ln for ln in lines if r.search(ln)]
    road = 'unknown'
    if hits(r'OVR hunted: begin'):
        road = 'hunted'
    elif hits(r'OVR massacre: begin'):
        road = 'massacre'
    elif hits(r'OVR stranger: offer REFUSED'):
        road = 'refuse'
    rows = GATE + COMMON + {'refuse': REFUSE, 'massacre': MASSACRE, 'hunted': HUNTED}.get(road, [])
    results = []
    for rid, rx, desc, must in rows:
        h = hits(rx)
        if must is None:
            status = 'INFO' if h else 'info'
        elif must:
            status = 'PASS' if h else 'FAIL'
        else:
            status = 'FAIL' if h else 'PASS'
        results.append({'row': rid, 'status': status, 'what': desc, 'matches': len(h), 'sample': h[:3]})
    # every scene that opened must have closed ok
    opened = re.findall(r'OVR scene: (macro|lua) (\S+)$', log, re.M)
    for kind, name in opened:
        ok = re.search(rf'OVR scene: {kind} {re.escape(name)} done ok=true', log)
        results.append({'row': f'scene {name}', 'status': 'PASS' if ok else 'FAIL',
                        'what': 'opened scene closed with ok=true', 'matches': 1 if ok else 0, 'sample': []})
    return results, road


def main() -> int:
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('log', nargs='?', type=pathlib.Path)
    a.add_argument('--bundle', type=pathlib.Path)
    a.add_argument('--json', type=pathlib.Path)
    args = a.parse_args()
    log_path = args.log or (args.bundle / 'NoviCompatibility/FableScriptExtender.log' if args.bundle else None)
    if not log_path or not log_path.exists():
        raise SystemExit(f'log not found: {log_path}')
    results, road = grade(log_path.read_text(encoding='utf-8', errors='replace'))
    print(f'log: {log_path}\nroad taken: {road}\n')
    failed = 0
    for r in results:
        print(f'{r["status"]:4s} {r["row"]:24s} {r["what"]}' + (f'  [{r["sample"][0][:90]}]' if r['sample'] and r['status'] != 'PASS' else ''))
        failed += r['status'] == 'FAIL'
    print('\nMANUAL (eyes only):')
    for rid, what in MANUAL:
        print(f'     {rid:24s} {what}')
    print(f'\n{failed} failed')
    if args.json:
        args.json.write_text(json.dumps({'road': road, 'results': results}, indent=1), encoding='utf-8')
    return 1 if failed else 0


if __name__ == '__main__':
    raise SystemExit(main())
