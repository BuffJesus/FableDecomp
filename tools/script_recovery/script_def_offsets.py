"""Retail `CScriptDef` field offsets (the global game data the scripts read through `DAT_0143e90c + off`).

Evidence: the Ego_r PDB layout of CScriptDef (ghidra_out/struct_layouts_egor.tsv, 959 fields, 4436 bytes) and the
retail script.bin SCRIPT_DEF entry 597 (def_schema.json field order + decoded values). The retail object is the PDB
layout without its 64-byte header, and with `std::vector<float>` / `<long>` / `<CDefString>` members 4 bytes
smaller (12 instead of the debug build's 16); anchors: `OVI_MoralityChangePerDeed` at 0xd64 (retail-byte decode in
refs/script_recovery/new_oakvale_intro/api_requirements.json), `GUI_MeleeBeetles` at 0xf10 (ScorpionHome reads the
scorpion count there; decodes 10.0), `AmbushScamRenown` at 0x270 (AddBoast).

    python tools/script_recovery/script_def_offsets.py            # writes refs/script_recovery/script_def_offsets.json
"""
from __future__ import annotations

import csv
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'refs/script_recovery/script_def_offsets.json'
HEADER = 64
SHRINKING = ('Vector_M', 'Vector_J', 'Vector_VCDefString__')      # 16 in Ego_r, 12 in retail


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
    table, shrink = {}, 0
    for off, ctype, name in pdb:
        value = decoded.get(name)
        table[f'{off - HEADER - shrink:#x}'] = {'name': name, 'type': ctype, 'defType': ftype.get(name),
                                                'value': value if isinstance(value, (int, float, str, bool)) else None}
        if ftype.get(name) in SHRINKING:
            shrink += 4
    assert table['0xd64']['name'] == 'OVI_MoralityChangePerDeed' and table['0xf10']['name'] == 'GUI_MeleeBeetles' \
        and table['0x270']['name'] == 'AmbushScamRenown', 'anchor mismatch'
    return table


def load():
    if not OUT.exists():
        return {}
    return json.loads(OUT.read_text(encoding='utf-8'))


if __name__ == '__main__':
    table = build()
    OUT.write_text(json.dumps(table, indent=1) + '\n', encoding='utf-8')
    print(f'{len(table)} fields -> {OUT}')
