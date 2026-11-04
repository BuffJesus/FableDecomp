"""Retail CScriptDef field metadata, recovered from native named-member transfers.

The PDB supplies names/types and script.bin supplies reference values for comments.
Retail transfer call sites establish offsets; no debug-layout subtraction is used.
Values remain runtime reads in generated Lua. See native_script_def_fields.py.
"""
from __future__ import annotations

import csv
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'refs/script_recovery/script_def_offsets.json'

def build():
    sys.path.insert(0, str(ROOT / 'tools'))
    from parse_frontend import load_all, resolve_type, decode_entry
    entries, schema, _ = load_all(str(ROOT / 'work/ui_proto/base'), str(ROOT / 'ghidra_out/def_schema.json'), 'script.bin')
    entry = next(e for e in entries if e['index'] == 597)
    fields = [f for f in schema[resolve_type(entry['definition'], schema)]['fields'] if f.get('name')]
    ftype = {f['name']: f['type'] for f in fields}
    decoded, _ = decode_entry(entry, schema)
    pdb = []
    for row in csv.reader(open(ROOT / 'ghidra_out/struct_layouts_egor.tsv', encoding='utf-8'), delimiter='\t'):
        if row and row[0] == 'CScriptDef' and len(row) >= 5 and row[4] != '_padding_':
            pdb.append((int(row[2]), row[3], row[4]))
    pdb.sort()
    from tools.script_recovery.native_script_def_fields import recover
    from tools.script_recovery.lift_native_lua import RData
    native = recover(RData(), {name for _, _, name in pdb})
    table = {}
    for pdb_offset, ctype, name in pdb:
        value = decoded.get(name)
        evidence = native[name]
        table[f'{evidence["offset"]:#x}'] = {
            'name': name, 'type': ctype, 'defType': ftype.get(name),
            'value': value if isinstance(value, (int, float, str, bool)) else None,
            'pdbOffset': pdb_offset, 'retailTransferCall': f'{evidence["call"]:#x}',
        }
    table = dict(sorted(table.items(), key=lambda row: int(row[0], 0)))
    return table


def load():
    if not OUT.exists():
        return {}
    return json.loads(OUT.read_text(encoding='utf-8'))


if __name__ == '__main__':
    table = build()
    OUT.write_text(json.dumps(table, indent=1) + '\n', encoding='utf-8')
    print(f'{len(table)} fields -> {OUT}')
