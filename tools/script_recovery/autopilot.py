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
`do` lines go to the channel (`input: <keys>` lines are reserved for the FableForge frontend driver and are
reported as SKIPPED here); `expect` is a regex the log must show after the step's batch marker within `timeout`
seconds; `forbid` is a regex that fails the step if it appears in the same window. The run stops at the first
failure, dumps every live quest, and archives the log through ab_playtest.collect.

The game is launched and the save loaded by the user or by ab_playtest/local_test (`--launch`); this driver only
talks to a running game. It never launches Fable itself.
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
    """Read new lines of the FSE log since the last call (the log is append-only while the game runs)."""

    def __init__(self, path: Path):
        self.path = path
        self.pos = path.stat().st_size if path.is_file() else 0

    def new_lines(self) -> list[str]:
        if not self.path.is_file():
            return []
        with self.path.open('r', encoding='utf-8', errors='replace') as f:
            f.seek(self.pos)
            data = f.read()
            self.pos = f.tell()
        return data.splitlines()


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


def run_checklist(bundle: str, steps: list[dict], default_timeout: float = 30.0) -> list[dict]:
    ch = Channel(bundle)
    results = []
    for step in steps:
        sid = step.get('id', f'step{len(results) + 1}')
        do = step.get('do', [])
        expect = step.get('expect')
        forbid = step.get('forbid')
        timeout = float(step.get('timeout', default_timeout))
        inputs = [d for d in do if isinstance(d, str) and d.startswith('input:')]
        commands = [d for d in do if isinstance(d, str) and not d.startswith('input:')]
        tail = LogTail(ch.log)
        outcome = {'id': sid, 'status': 'PASS', 'detail': ''}
        if inputs:
            outcome.update(status='SKIPPED', detail='input steps need the FableForge frontend driver: ' + '; '.join(inputs))
            results.append(outcome)
            print(f'{sid}: SKIPPED ({outcome["detail"]})')
            continue
        # `repeat`: re-send the commands every `interval` seconds until the marker shows (a respawning spawner)
        repeats = int(step.get('repeat', 1))
        interval = float(step.get('interval', 2.0))
        replies = ch.send(commands, timeout=min(timeout, 15.0)) if commands else []
        errors = [r for r in replies if 'error' in r or 'exception' in r or 'TIMEOUT' in r]
        if errors:
            outcome.update(status='FAIL', detail='channel: ' + ' | '.join(errors))
        elif expect:
            deadline = time.time() + timeout
            seen = ''
            sent, next_send = 1, time.time() + interval
            while time.time() < deadline and outcome['status'] == 'PASS':
                chunk = tail.new_lines()
                seen += '\n'.join(chunk) + '\n'
                if forbid and re.search(forbid, seen):
                    outcome.update(status='FAIL', detail=f'forbidden marker /{forbid}/ appeared')
                    break
                if re.search(expect, seen):
                    outcome['detail'] = f'expected /{expect}/ seen' + (f' after {sent} send(s)' if repeats > 1 else '')
                    break
                if commands and sent < repeats and time.time() >= next_send:
                    ch.send(commands, timeout=min(timeout, 15.0))
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
    r = sub.add_parser('run'); r.add_argument('bundle'); r.add_argument('checklist'); r.add_argument('--report')
    t = sub.add_parser('tail'); t.add_argument('bundle')
    a = ap.parse_args()
    if a.cmd == 'send':
        for line in Channel(a.bundle).send(a.lines, timeout=a.timeout):
            print(line)
    elif a.cmd == 'run':
        steps = json.loads(Path(a.checklist).read_text(encoding='utf-8'))
        results = run_checklist(a.bundle, steps)
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
