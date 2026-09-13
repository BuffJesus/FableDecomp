"""Compare captured native RUN1 dispatch with its decoded retail SkipCond."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

try:
    from tools.script_recovery.analyze_cutscene_task_capture import analyze_text
except ModuleNotFoundError:
    from analyze_cutscene_task_capture import analyze_text


def audit(normal: Path, early: Path, macros: Path) -> dict:
    inputs = {}
    texts = {}
    runs = {}
    for name, path in [('userReportedUnskipped', normal), ('earlySkip', early)]:
        raw = path.read_bytes()
        inputs[name] = {'path': str(path), 'sha256': hashlib.sha256(raw).hexdigest().upper()}
        texts[name] = raw.decode('utf-8', errors='replace')
        candidates = [r for r in analyze_text(texts[name])['runs'] if r['macro'] == 'CS_OAKVALEINTRO_BULLYRUN1']
        if len(candidates) != 1:
            raise ValueError(f'{name}: expected exactly one complete RUN1 capture')
        runs[name] = candidates[0]
    authored = next(m for m in json.loads(macros.read_text()) if m['name'] == 'CS_OAKVALEINTRO_BULLYRUN1')
    inputs['macros'] = {'path': str(macros), 'sha256': hashlib.sha256(macros.read_bytes()).hexdigest().upper()}
    baseline, skipped = runs['userReportedUnskipped'], runs['earlySkip']
    skip_line = skipped['skipQueries'][0]['line'] if skipped['skipQueries'] else 10**9
    tail = [c['command'] for c in skipped['commands'] if c['line'] > skip_line]
    checks = {
        'bothNativeMacrosReturned': baseline['ended'] and skipped['ended'],
        'baselineExecutedNativeRunTo': any(c['command'].startswith('BULLY.RunTo ') for c in baseline['commands']),
        'baselineIssuedMovementOnBullyController': baseline['bullyBoundary']['issueCount'] > 0,
        'baselineHadNoLuaFallback': 'native RunTo incomplete; completing authored marker move' not in texts['userReportedUnskipped'],
        'earlySkipQueryObserved': bool(skipped['skipQueries']),
        'earlySkipCommandsMatchRetailSkipCondExactly': tail == authored['skipCommands'],
        'earlySkipNeverDispatchedRunTo': not any(c['command'].startswith('BULLY.RunTo ') for c in skipped['commands']),
        'earlySkipHadNoNativeMovementIssueOnBully': skipped['bullyBoundary']['controller'] is not None and skipped['bullyBoundary']['issueCount'] == 0,
        'earlySkipEndedAtBully1': 'BullyRun after1 bully=(3604.479,860.742,13.707)' in texts['earlySkip'],
        'luaFallbackRanAfterSkip': 'BullyRun native RunTo incomplete; completing authored marker move' in texts['earlySkip'],
        'fallbackReachedBully2': 'BullyRun compatibility end frames=46 bully=(3596.740,851.895,13.375)' in texts['earlySkip'],
    }
    return {'schema': 'bully-skip-runtime-audit/0.1', 'ok': all(checks.values()), 'checks': checks,
            'inputs': inputs, 'earlySkipExecutedTail': tail,
            'limits': 'Observed command dispatch and endpoint evidence; visual fix and audio require separate live verification. A true engine skip query alone does not prove user input.'}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--normal', type=Path, required=True)
    parser.add_argument('--early', type=Path, required=True)
    parser.add_argument('--macros', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.normal, args.early, args.macros)
    args.output.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'ok': result['ok'], 'checks': result['checks']}, indent=2))
    raise SystemExit(0 if result['ok'] else 1)
