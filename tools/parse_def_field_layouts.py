#!/usr/bin/env python3
"""Parse def field layouts from the batch-decompiled Transfer methods.

Each CxxxDef::Transfer streams its persisted fields through a CPersistContext.
In the decompile this shows as repeated persist-helper calls taking `this + 0xNN`
(shown as in_ECX/param_1/this depending on how much signature Ghidra recovered).
The set of offsets IS the def payload layout; consecutive offsets give field
sizes; the persist-helper called per field is a type discriminator.

Validated against the hand-mapped CChestDef (offsets 0x25 0x28 0x2c 0x30 0x34
0x38).

Outputs:
  ghidra_out/def_field_layouts.json  - {def: {addr, fields:[{off,size,helper}]}}
  ghidra_out/def_field_layouts.tsv   - flat def\toffset\tsize\thelper
"""
import json
import re
from collections import defaultdict
from pathlib import Path

SRC = Path(r"D:\Documents\FableTLC\ghidra_out\def_transfers.c")
OUT_JSON = Path(r"D:\Documents\FableTLC\ghidra_out\def_field_layouts.json")
OUT_TSV = Path(r"D:\Documents\FableTLC\ghidra_out\def_field_layouts.tsv")

HEADER_RE = re.compile(r"^//=== (\S+) @ ([0-9a-fA-F]+) ===")
# base identifier + hex or decimal offset, e.g. "in_ECX + 0x34" or "this + 100"
OFF_RE = re.compile(r"\b([A-Za-z_]\w*)\s*\+\s*(0x[0-9a-fA-F]+|\d+)\b")
# function called on the line (last IDENT before a '(')
CALL_RE = re.compile(r"([A-Za-z_][\w:<>~]*)\s*\(")


def parse_offset(tok):
    return int(tok, 16) if tok.startswith("0x") else int(tok)


def main():
    text = SRC.read_text(encoding="utf-8", errors="replace")
    lines = text.splitlines()

    # Split into per-def blocks.
    blocks = []  # (name, addr, [lines])
    cur = None
    for ln in lines:
        m = HEADER_RE.match(ln)
        if m:
            if cur:
                blocks.append(cur)
            cur = (m.group(1), m.group(2), [])
        elif cur:
            cur[2].append(ln)
    if cur:
        blocks.append(cur)

    result = {}
    for name, addr, body in blocks:
        # Gather (base, offset, helperOnLine) for every offset expression.
        per_base = defaultdict(list)  # base -> list of (offset, helper)
        for ln in body:
            call = None
            cm = CALL_RE.search(ln)
            if cm:
                call = cm.group(1)
            for bm in OFF_RE.finditer(ln):
                base = bm.group(1)
                off = parse_offset(bm.group(2))
                # Ignore absurd offsets (likely constants, not field access).
                if off > 0x4000:
                    continue
                per_base[base].append((off, call))
        if not per_base:
            result[name] = {"addr": addr, "fields": [], "note": "no offsets"}
            continue
        # `this` = the base register with the most distinct field offsets.
        base = max(per_base, key=lambda b: len({o for o, _ in per_base[b]}))
        seen = {}
        for off, helper in per_base[base]:
            # First helper wins for a given offset.
            seen.setdefault(off, helper)
        offs = sorted(seen)
        fields = []
        for i, off in enumerate(offs):
            size = (offs[i + 1] - off) if i + 1 < len(offs) else 0
            fields.append({"off": off, "size": size, "helper": seen[off]})
        result[name] = {"addr": addr, "base": base, "fields": fields}

    OUT_JSON.write_text(json.dumps(result, indent=1), encoding="utf-8")
    with OUT_TSV.open("w", encoding="utf-8", newline="") as f:
        f.write("def\taddr\toffset\tsize\thelper\n")
        for name, info in result.items():
            for fld in info.get("fields", []):
                f.write(f"{name}\t{info['addr']}\t0x{fld['off']:x}\t"
                        f"{fld['size']}\t{fld['helper']}\n")

    total_fields = sum(len(v.get("fields", [])) for v in result.values())
    with_fields = sum(1 for v in result.values() if v.get("fields"))
    print(f"defs: {len(result)}  with fields: {with_fields}  "
          f"total fields: {total_fields}")

    # Validate against CChestDef ground truth.
    chest = result.get("CChestDef", {})
    chest_offs = [f"0x{f['off']:x}" for f in chest.get("fields", [])]
    print(f"CChestDef offsets: {chest_offs} "
          f"(expected 0x25 0x28 0x2c 0x30 0x34 0x38)")
    # Top defs by field count.
    top = sorted(result.items(), key=lambda kv: -len(kv[1].get("fields", [])))[:12]
    print("largest defs:")
    for name, info in top:
        print(f"  {name}: {len(info.get('fields', []))} fields")


if __name__ == "__main__":
    main()
