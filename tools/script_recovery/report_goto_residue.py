"""Why every `-- TODO(native): goto X` in the lifted drafts is still there.

The lifter writes that marker when it cannot express a native jump in Lua and falls back to `return`.
`native_cleanup_regions` removes the marker wherever the jump's target is an epilogue it can hoist, so what
remains is the real residue -- and it is not one problem. A site whose target region has an EMPTY body drops
no cleanup at all (only the jump itself is missing); a site whose target has statements is a leak.

    python tools/script_recovery/report_goto_residue.py [--lifted refs/script_recovery/lifted]
"""
from __future__ import annotations

import argparse
import collections
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))

from tools.script_recovery import native_cleanup_regions as ncr  # noqa: E402

TODO_GOTO = re.compile(r'-- TODO\(native\): goto (\w+)')


def classify(chunk):
    """(reason, target) for every unexpressible jump in one function chunk."""
    lines = chunk.split('\n')
    regions = ncr._regions(lines)
    emitted = ncr._emitted_labels(lines)
    marked = {m.group('label') for line in lines for m in [ncr.LABEL_LINE.match(line)] if m}
    for line in lines:
        m = TODO_GOTO.search(line)
        if not m:
            continue
        target = m.group(1)
        if target in regions:
            body = regions[target][2]
            yield ('region kept, nothing to run' if not body else 'region kept with statements'), target
        elif target in emitted:
            yield 'target is a real ::label::, no region', target
        elif target in marked:
            yield 'target has a LAB comment but no straight-line region', target
        else:
            yield 'target is not in this function', target


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--lifted', type=Path, default=ROOT / 'refs' / 'script_recovery' / 'lifted')
    ap.add_argument('--json', action='store_true')
    args = ap.parse_args()

    counts, per_unit, examples = collections.Counter(), collections.Counter(), {}
    for path in sorted(args.lifted.glob('*/draft/**/*.lua')):
        unit = path.relative_to(args.lifted).parts[0]
        for chunk in re.split(r'(?m)^(?=function )', path.read_text(encoding='utf-8')):
            if not chunk.startswith('function '):
                continue
            for reason, target in classify(chunk):
                counts[reason] += 1
                per_unit[unit] += 1
                examples.setdefault(reason, f'{path.relative_to(args.lifted)}: goto {target}')
    if args.json:
        print(json.dumps({'reasons': counts, 'units': per_unit, 'examples': examples}, indent=2))
        return
    print(f'{sum(counts.values())} unexpressible jumps left')
    for reason, n in counts.most_common():
        print(f'  {n:5d}  {reason}')
        print(f'         e.g. {examples[reason]}')
    for unit, n in per_unit.most_common():
        print(f'  {n:5d}  in {unit}')


if __name__ == '__main__':
    main()
