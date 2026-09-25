"""Run quest drivers in order, using only each successful run's harvested save.

Each stage launches a fresh game, restores the protected profile on exit, and
passes its new checkpoint to the next stage. No activation or outcome flags
are injected by this wrapper.
"""
from __future__ import annotations

import argparse
from datetime import datetime
import json
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.autopilot import SAVES


def run(campaign: Path, *, bundle: str, tag: str, save: str | None = None,
        start_at: str | None = None) -> int:
    spec = json.loads(campaign.read_text(encoding='utf-8'))
    stages = spec['stages']
    if not re.fullmatch(r'[A-Za-z0-9_-]+', tag):
        raise ValueError('tag must contain only letters, numbers, underscores, and hyphens')
    names = [stage['quest'] for stage in stages]
    if len(set(names)) != len(names):
        raise ValueError('campaign quest names must be unique')
    if start_at:
        if not save:
            raise ValueError('--start-at requires --save for that stage')
        stages = stages[names.index(start_at):]
    source = save or spec['save']
    if not (SAVES / source / 'AutoSave').is_file():
        raise FileNotFoundError(f'no AutoSave in {SAVES / source}')
    for stage in stages:
        if not re.fullmatch(r'[a-z0-9_]+', stage['quest']):
            raise ValueError('invalid quest configuration name')
        path = Path(__file__).parent / 'runner_quests' / (stage['quest'] + '.json')
        json.loads(path.read_text(encoding='utf-8'))
        if (SAVES / f'{tag}_{stage["quest"]}' / 'AutoSave').exists():
            raise FileExistsError(f'checkpoint already exists for {tag}_{stage["quest"]}; use a new tag')
    output = ROOT / 'work/runner'
    output.mkdir(parents=True, exist_ok=True)
    report_path = output / f'{tag}_campaign.json'
    if report_path.exists():
        raise FileExistsError(f'{report_path} exists; use a new tag')
    report = {'campaign': str(campaign), 'bundle': bundle, 'source': source, 'stages': []}
    for stage in stages:
        quest = stage['quest']
        checkpoint = f'{tag}_{quest}'
        row = {'quest': quest, 'source': source, 'checkpoint': checkpoint, 'status': 'running'}
        report['stages'].append(row)
        report_path.write_text(json.dumps(report, indent=2), encoding='utf-8')
        config = Path(__file__).parent / 'runner_quests' / (quest + '.json')
        cmd = [sys.executable, '-u', str(Path(__file__).with_name('ingame_runner.py')), str(config),
               '--bundle', bundle, '--tag', checkpoint, '--minutes', str(stage.get('minutes', 30)),
               '--launch', '--save', source, '--harvest', checkpoint]
        print(f'Campaign: {quest} from {source}', flush=True)
        result = subprocess.run(cmd, cwd=ROOT)
        row['exitCode'] = result.returncode
        row['status'] = 'done' if result.returncode == 0 and (SAVES / checkpoint / 'AutoSave').is_file() else 'failed'
        report_path.write_text(json.dumps(report, indent=2), encoding='utf-8')
        if row['status'] != 'done':
            print(f'Campaign stopped at {quest}; report: {report_path}', flush=True)
            return 1
        source = checkpoint
    print(f'Campaign complete; checkpoint: {source}; report: {report_path}', flush=True)
    return 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('campaign', type=Path)
    parser.add_argument('--bundle', default='v15')
    parser.add_argument('--tag', default='campaign_' + datetime.now().strftime('%Y%m%d_%H%M%S'))
    parser.add_argument('--save')
    parser.add_argument('--start-at')
    args = parser.parse_args()
    return run(args.campaign, bundle=args.bundle, tag=args.tag, save=args.save, start_at=args.start_at)


if __name__ == '__main__':
    sys.exit(main())
