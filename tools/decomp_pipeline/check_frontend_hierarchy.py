#!/usr/bin/env python3
"""Compare presenter animation timing to real coastal component hierarchies."""
import json
from check_cgame_play import ROOT, parity, run
from oracle_frontend_hierarchy import FrontendHierarchyOracle


def main():
    directory = ROOT/'work/frontend_hierarchy_check'; directory.mkdir(parents=True, exist_ok=True)
    # Exact quarter-second steps avoid a clock-accumulation comparison; include
    # large stalls, zero-delta updates, and uninterrupted runs through many swaps.
    times = [i*0.25 for i in range(256)]+[100, 100, 100.25, 500, 500.25]+[500.5+i*0.25 for i in range(256)]
    inputs = directory/'times.txt'; inputs.write_text('\n'.join(map(str, times))+'\n')
    env = parity.env(); obj, exe = directory/'behavior.obj', directory/'behavior.exe'
    run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy', '/Fo'+str(obj),
         ROOT/'rebuild/tests/integration/FrontendHierarchy_test.cpp'], env)
    run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), obj], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(times)*2: raise RuntimeError('Incomplete hierarchy comparison')
    oracle = FrontendHierarchyOracle(685)
    errors, previous, worst = [], 0, 0
    for frame, time in enumerate(times):
        oracle.update(time-previous)
        for group, key in enumerate((686, 687)):
            expected = oracle.sample(key)
            actual = list(map(int, lines[frame*2+group].split()))
            error = max(abs(a-b) for a,b in zip(actual[4:], expected[4:])); worst = max(worst, error)
            if actual[:4] != expected[:4] or error > 1:
                errors.append({'frame': frame, 'group': group, 'actual': actual, 'retail': expected})
        previous = time
    report = {'accepted': not errors, 'frames_per_group': len(times), 'groups': 2,
              'nodes': len(oracle.nodes), 'max_alpha_error': worst, 'errors': errors,
              'scope': 'controlled definition-derived 52-node coastal root with both swapping groups; native base/state/colour/swap bodies; lookup and allocator doubles',
              'excludes': 'constructor chain, root manager scale, rendering, actions and nonempty deletion requests'}
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f"FRONTEND_HIERARCHY {'PASS' if not errors else 'FAIL'} frames={len(times)*2} max_alpha_error={worst} failures={len(errors)}")
    return int(bool(errors))


if __name__ == '__main__': raise SystemExit(main())
