"""Inventory readability debt in every current Lua port, including older FSE packages.

This is a mechanical review queue, not a claim of human-readable or correct behavior.
Comments and strings do not count as executable decompiler residue; unresolved TODO
comments are recorded separately so deleting diagnostics cannot masquerade as recovery.
"""
from __future__ import annotations

import argparse
from collections import Counter
import json
from pathlib import Path
import re

from tools.script_recovery.readable_lua import tokens

ROOT = Path(__file__).resolve().parents[2]
IDENTIFIERS = {
    'nativeNames': re.compile(r'(?:(?:\w*_)?(?:LAB|lab)_[0-9a-fA-F]+\w*|FUN_[0-9a-fA-F]+|DAT_[0-9a-fA-F]+|helper_[0-9a-fA-F]{6,}|self_0x[0-9a-fA-F]+|self0X[0-9a-fA-F]+)\Z'),
    'machineTemporaries': re.compile(r'(?:[a-zA-Z]{1,3}Var\d+(?:_\d+)*|[a-zA-Z]*Stack_\w+|[a-zA-Z]_stk_\w+|local_[0-9a-fA-F]+|unaff_\w+|in_stack_\w+|native_arg_\w+|extraout_\w+)\Z'),
    'genericTemporaries': re.compile(r'(?:scratchValue\d*|predicateResult\d*|flag[23]UVar\d*|r\d+(?:_\d+)*)\Z'),
}


def inspect_source(source):
    findings, line = [], 1
    for token in tokens(source):
        value, kind = token[0], token.lastgroup
        category = None
        if kind == 'identifier':
            category = next((name for name, pattern in IDENTIFIERS.items() if pattern.fullmatch(value)), None)
            if value == 'goto':
                category = 'gotos'
        elif kind in ('comment', 'longcomment'):
            if re.search(r'\b(?:TODO|FIXME|UNRESOLVED|UNSUPPORTED)\b', value, re.I):
                category = 'unresolvedDiagnostics'
        if category:
            findings.append({'line': line, 'category': category, 'text': value.strip()})
        line += value.count('\n')
    return {'lines': len(source.splitlines()), 'counts': dict(Counter(f['category'] for f in findings)),
            'status': 'needs-recovery' if any(f['category'] == 'unresolvedDiagnostics' for f in findings)
            else 'needs-readability-work' if findings else 'needs-human-review', 'findings': findings}


def current_scripts(root=ROOT):
    lifted = root / 'refs/script_recovery/lifted'
    for package in sorted(lifted.iterdir()):
        if package.is_dir():
            for stage in sorted(package.iterdir()):
                if stage.is_dir() and (stage.name.startswith('readable') or stage.name == 'FSE'):
                    yield from sorted(stage.rglob('*.lua'))
    yield from sorted((root / 'refs/script_recovery/authored').rglob('*.lua'))


def audit(root=ROOT):
    from lupa.lua54 import LuaRuntime
    lua = LuaRuntime()
    rows = {p.relative_to(root).as_posix(): inspect_source(p.read_text(encoding='utf-8-sig'))
            for p in current_scripts(root)}
    for path, row in rows.items():
        try:
            lua.compile((root / path).read_text(encoding='utf-8-sig'), name=path)
            row['syntax'] = {'ok': True}
        except Exception as exc:
            row['syntax'] = {'ok': False, 'error': str(exc)}
            row['status'] = 'invalid-lua'
    totals = Counter()
    for row in rows.values():
        totals.update(row['counts'])
    return {'schema': 'lua-readability-audit/1',
            'scope': 'All lifted readable*, lifted FSE, and authored Lua; draft/evidence/candidates excluded.',
            'qualification': 'Mechanical findings only. No file is certified fully readable by this audit.',
            'files': rows, 'summary': {'files': len(rows), 'syntaxFailures': sum(not r['syntax']['ok'] for r in rows.values()),
                                      'statuses': dict(Counter(r['status'] for r in rows.values())),
                                      'counts': dict(totals)}}


def markdown(report):
    lines = ['# Current Lua readability review', '', report['scope'], '', report['qualification'], '',
             f"Files: {report['summary']['files']}; syntax failures: {report['summary']['syntaxFailures']}.", '',
             '| Script | Status | Jumps | Native names | Machine locals | Generic locals | Unresolved |',
             '| --- | --- | ---: | ---: | ---: | ---: | ---: |']
    for path, row in report['files'].items():
        counts = row['counts']
        cells = [str(counts.get(k, 0)) for k in ('gotos', 'nativeNames', 'machineTemporaries', 'genericTemporaries', 'unresolvedDiagnostics')]
        lines.append('| ' + ' | '.join([path.removeprefix('refs/script_recovery/'), row['status'], *cells]) + ' |')
    return '\n'.join(lines) + '\n'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    report = audit()
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    args.out.with_suffix('.md').write_text(markdown(report), encoding='utf-8')
    print(json.dumps(report['summary'], indent=2))


if __name__ == '__main__':
    main()
