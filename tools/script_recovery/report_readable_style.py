"""How far the readable units still are from a hand-written quest script.

The style target is Aeon's ports (`work/aeon_lua_ports/`): named locals, no `goto`, no decompiler spellings.
This counts what is left, over CODE ONLY — comments and string bodies are blanked first, so the provenance
headers (`-- Main (retail 0x00d52e90)`) and the `-- TODO(native)` gap markers do not inflate the numbers.

    python tools/script_recovery/report_readable_style.py [--json]
"""
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LIFTED = ROOT / 'refs' / 'script_recovery' / 'lifted'

# the readable stage of each converter unit (NewOakValeIntro has its own hand-built pipeline)
UNITS = (('GuildTraining', 'readable_converter'), ('TraderConflict', 'readable'), ('OrchardFarm', 'readable'))

PATTERNS = (
    ('goto / ::label::',      r'\bgoto \w+|::\w+::'),
    ('scratchValue',          r'\bscratchValue\d*\b'),
    ('raw slot name',         r'\b(?:[a-zA-Z]*Stack_[0-9a-f]+|\w*_stk_[0-9a-f]+|ctr_[0-9a-f]+|local_[0-9a-f]+)\w*\b'),
    ('Ghidra register name',  r'\b(?:[a-z]{1,2}Var\d+|fret_\d+|r\d+|p\d+|this_\d+)\w*\b'),
    ('predicateResult',       r'\bpredicateResult\d*\b'),
    ('converter scaffolding', r'__native_\w+|__region_\w+|__cleanup_\w+'),
    ('hex literal in code',   r'0x[0-9a-f]{2,}'),
    ('bitwise op',            r'[^&|]\s[&|]\s[^&|]'),
    ('free global',           r'\bunaff_\w+|__unknown_push'),
)


def code_only(text):
    text = re.sub(r'--\[\[.*?\]\]', '', text, flags=re.S)
    text = re.sub(r'(?m)--[^\n]*', '', text)
    return re.sub(r'"(?:[^"\\]|\\.)*"', '""', text)


def measure(package, stage):
    files = sorted((LIFTED / package / stage).rglob('*.lua'))
    raw = '\n'.join(f.read_text(encoding='utf-8') for f in files)
    code = code_only(raw)
    lines = raw.count('\n')
    out = {'files': len(files), 'lines': lines, 'todoNative': len(re.findall(r'-- TODO\(native\)', raw)), 'residue': {}}
    for name, pattern in PATTERNS:
        n = len(re.findall(pattern, code))
        if n:
            out['residue'][name] = {'count': n, 'per1000Lines': n * 1000 // max(lines, 1)}
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--json', action='store_true')
    args = ap.parse_args()
    report = {f'{p}/{s}': measure(p, s) for p, s in UNITS if (LIFTED / p / s).exists()}
    if args.json:
        print(json.dumps(report, indent=2))
        return
    for unit, r in report.items():
        print(f"\n== {unit}  ({r['files']} files, {r['lines']} lines, {r['todoNative']} TODO(native))")
        for name, v in sorted(r['residue'].items(), key=lambda kv: -kv[1]['per1000Lines']):
            print(f"   {v['count']:6d}  {name:24s} {v['per1000Lines']:4d} per 1000 lines")


if __name__ == '__main__':
    main()
