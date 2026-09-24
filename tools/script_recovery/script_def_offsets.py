"""Retail `CScriptDef` field offsets (the global game data the scripts read through `DAT_0143e90c + off`).

Evidence: the Ego_r PDB layout of CScriptDef (ghidra_out/struct_layouts_egor.tsv, 959 fields, 4436 bytes) and the
retail script.bin SCRIPT_DEF entry 597 (def_schema.json field order + decoded values). From the 0xd64 anchor on,
the retail object is the PDB layout without its first 64 bytes, and with `std::vector<float>` / `<long>` /
`<CDefString>` members 4 bytes smaller (12 instead of the debug build's 16); anchors: `OVI_MoralityChangePerDeed`
at 0xd64 (retail-byte decode in refs/script_recovery/new_oakvale_intro/api_requirements.json), `GUI_MeleeBeetles`
at 0xf10 (ScorpionHome reads the scorpion count there; decodes 10.0). The leading block up to +0x254 is PDB - 4
(boast reads of four units, see EARLY_HEADER); fields between the two proven regions are flagged
`verified: false` and the readable output leaves their offsets numeric.

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
# The leading block is PDB - 4, not PDB - 64 (2026-09-24): retail Orchard Good Init 0x00DD23A0 reads its Naked
# boast at +0x180/+0x184 = OFGoodNakedBoastCost/Reward (PDB 0x184); the live Orchard Evil boast UI shows
# 80/160, 100/400, 100/300 read from +0x168..+0x17c (script.bin OFEvil* values); Trader Conflict reads its boasts
# at +0x1c8.., Trader Escort at +0x1f8.., Wasp at +0x210.. -- every one exactly PDB - 4. The old `0x270 =
# AmbushScamRenown` anchor has no reader in any typed export. Proven up to +0x254 (Trader Conflict's TCE reward
# ints); the late model is proven from the 0xd64 decode anchor on. The 0x3c-byte change lies in between.
EARLY_HEADER = 4
EARLY_VERIFIED_END = 0x254
LATE_VERIFIED_START = 0xd64
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
        entry = {'name': name, 'type': ctype, 'defType': ftype.get(name),
                 'value': value if isinstance(value, (int, float, str, bool)) else None}
        late = off - HEADER - shrink
        early = off - EARLY_HEADER - shrink
        if early <= EARLY_VERIFIED_END:
            table[f'{early:#x}'] = entry
        elif late >= LATE_VERIFIED_START:
            table[f'{late:#x}'] = entry
        elif late <= EARLY_VERIFIED_END:
            pass        # its late-model offset is inside the proven early block, where another field lives: unknown
        else:
            # between the two proven regions the retail layout loses 0x3c bytes somewhere; the late model's
            # offset is kept as the best guess, flagged so the readable output does not print the name
            table[f'{late:#x}'] = dict(entry, verified=False)
        if ftype.get(name) in SHRINKING:
            shrink += 4
    assert table['0xd64']['name'] == 'OVI_MoralityChangePerDeed' and table['0xf10']['name'] == 'GUI_MeleeBeetles' \
        and table['0x168']['name'] == 'OFEvilNakedBoastCost' and table['0x180']['name'] == 'OFGoodNakedBoastCost' \
        and table['0x1c8']['name'] == 'TraderConflictEvilNakedBoastCost', 'anchor mismatch'
    return table


def load():
    if not OUT.exists():
        return {}
    return json.loads(OUT.read_text(encoding='utf-8'))


if __name__ == '__main__':
    table = build()
    OUT.write_text(json.dumps(table, indent=1) + '\n', encoding='utf-8')
    print(f'{len(table)} fields -> {OUT}')
