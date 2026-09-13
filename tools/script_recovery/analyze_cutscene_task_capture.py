"""Summarize passive cutscene task traces without attributing foreign polls to Bully."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path


def analyze_text(text: str) -> dict:
    actors = {}
    runs = []
    current = None
    for line_number, line in enumerate(text.splitlines(), 1):
        actor = re.search(r"Actor identity '([^']+)':.*resourceData=(0x[0-9a-fA-F]+)", line)
        if actor:
            actors[actor[1]] = actor[2].lower()
        begin = re.search(r'\[CutsceneSkipDiag\] begin macro=(\S+)', line)
        if begin:
            current = {'macro': begin[1], 'beginLine': line_number,
                       'actors': dict(actors), 'controllers': {}, 'speech': [],
                       'commands': [], 'skipQueries': [], 'ended': False}
            actors = {}
            runs.append(current)
        if current is None:
            continue
        command = re.search(r'\[CutsceneCommandDiag\] macro=(\S+) tick=(\d+) command=(.*)', line)
        if command and command[1] == current['macro']:
            current['commands'].append({'line': line_number, 'tick': int(command[2]), 'command': command[3]})
        skip = re.search(r'\[CutsceneCommandDiag\] skipQueryTrue macro=(\S+) caller=(0x[0-9a-fA-F]+) tick=(\d+)', line)
        if skip and skip[1] == current['macro']:
            current['skipQueries'].append({'line': line_number, 'caller': skip[2], 'tick': int(skip[3])})
        task = re.search(r'\[CutsceneMoveDiag\] (issueThing|issue|poll) macro=(\S+) controller=(0x[0-9a-fA-F]+)(.*)', line)
        if task and task[2] == current['macro']:
            controller = task[3].lower()
            row = current['controllers'].setdefault(controller, {'issues': [], 'pollCount': 0, 'activePollCount': 0, 'transitions': []})
            if task[1] != 'poll':
                row['issues'].append({'line': line_number, 'kind': task[1], 'operands': task[4].strip()})
            else:
                active = re.search(r'active=([01])', task[4])
                if active:
                    value = int(active[1])
                    row['pollCount'] += 1
                    row['activePollCount'] += value
                    if not row['transitions'] or row['transitions'][-1]['active'] != value:
                        row['transitions'].append({'line': line_number, 'active': value})
        if '[CutsceneSpeechDiag] macro=' + current['macro'] + ' ' in line:
            current['speech'].append({'line': line_number, 'operands': line.split('[CutsceneSpeechDiag] ', 1)[1]})
        if '[CutsceneSkipDiag] end macro=' + current['macro'] + ' ' in line:
            current['ended'] = True
            current['endLine'] = line_number
            current = None
    for run in runs:
        for controller, row in run['controllers'].items():
            row['actorNames'] = [name for name, identity in run['actors'].items() if identity == controller]
        bully = run['actors'].get('BULLY')
        row = run['controllers'].get(bully, {})
        run['bullyBoundary'] = {
            'controller': bully,
            'issueCount': len(row.get('issues', [])),
            'pollCount': row.get('pollCount', 0),
            'interpretation': 'Observed calls only. No issue does not prove rejection; no actor-matched poll does not prove task completion. Polls may also belong to speech or other tasks.'}
    return {'schema': 'cutscene-task-capture/0.1', 'runs': runs}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('log', type=Path)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    raw = args.log.read_bytes()
    report = analyze_text(raw.decode('utf-8', errors='replace'))
    report.update(log=str(args.log), sha256=hashlib.sha256(raw).hexdigest().upper())
    payload = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(payload, encoding='utf-8')
    else:
        print(payload, end='')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
