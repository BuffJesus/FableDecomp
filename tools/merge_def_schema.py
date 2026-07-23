#!/usr/bin/env python3
"""Build the game.bin definition schema from the donor Transfer methods.

The FableWin donor Transfer methods carry, per field, the NAME (string literal),
the TYPE (encoded in the persist-helper mangling), and the CALL ORDER. Because
CPersistContext serializes fields in call order, the donor call order IS the
game.bin payload field order (note: NOT the memory-offset order — CChestDef
persists 0x28 before 0x25). Retail strips names/types but is otherwise the same
logic, so the donor schema decodes retail game.bin payloads field-by-field.

Retail memory offsets (from the retail Transfer sweep, def_field_layouts.json)
are attached only where the donor and retail field counts agree, as a secondary
hint for runtime/decomp use.

Validated on CChestDef: OpenParticleEffect(int32) PersistOnOpening(bool)
DisplayMessageOnEmpty(bool) OpenAnimationForCreature(CCharString)
OpenerObject(int32) OpenersRequired(int32).

Output: ghidra_out/def_schema.json + FableForge docs/re_reference copy.
"""
import json
import re
from pathlib import Path

DONOR = Path(r"D:\Documents\FableTLC\ghidra_out\donor_def_transfers.c")
RETAIL_JSON = Path(r"D:\Documents\FableTLC\ghidra_out\def_field_layouts.json")
OUT = Path(r"D:\Documents\FableTLC\ghidra_out\def_schema.json")
FF_COPY = Path(r"D:\Code\FableForge\docs\re_reference\def_schema.json")

HEADER_RE = re.compile(r"//=== (\S+) @ ([0-9a-fA-F]+) ===")
# ___Transfer[VectorOfSubComponents]_<TYPE>_CPersistContext...( "Name" | &DAT , ... this+0xNN )
# \s* spans the newline the decompiler inserts before the arg list.
CALL_RE = re.compile(
    r"___Transfer(VectorOfSubComponents)?_?([A-Za-z0-9_]*?)_CPersistContext\w*"
    r"\s*\(\s*([^,]+?)\s*,"
    r"[^;]*?[A-Za-z_]\w*\s*\+\s*(0x[0-9a-fA-F]+|\d+)",
    re.DOTALL)

BUILTIN = {
    "D": "char", "E": "uint8", "F": "int16", "G": "uint16",
    "H": "int32", "I": "uint32", "J": "int32", "K": "uint32",
    "M": "float", "N": "double", "_N": "bool", "_J": "int64", "_K": "uint64",
}


def decode_type(is_vector, code):
    if is_vector:
        m = re.match(r"V(C\w+?)__", code)
        return f"vector<{m.group(1)}>" if m else "vector"
    if code in BUILTIN:
        return BUILTIN[code]
    if code.startswith("W4"):
        m = re.match(r"W4(E\w+?)__", code)
        return f"enum {m.group(1)}" if m else "enum"
    m = re.match(r"V(C\w+?)__", code)
    if m:
        return m.group(1)
    return code or "?"


def parse_offset(tok):
    return int(tok, 16) if tok.startswith("0x") else int(tok)


def clean_name(raw):
    m = re.match(r'"([^"\\]+)"', raw.strip())
    return m.group(1) if m else None


def split_blocks(text):
    """Yield (name, addr, blockText) per def."""
    parts = list(HEADER_RE.finditer(text))
    for i, m in enumerate(parts):
        end = parts[i + 1].start() if i + 1 < len(parts) else len(text)
        yield m.group(1), m.group(2), text[m.end():end]


def main():
    donor_text = DONOR.read_text(encoding="utf-8", errors="replace")
    retail = json.loads(RETAIL_JSON.read_text(encoding="utf-8"))

    schema = {}
    total_fields = 0
    named_fields = 0
    with_offsets = 0

    for name, addr, block in split_blocks(donor_text):
        fields = []
        for m in CALL_RE.finditer(block):
            is_vector = m.group(1) is not None
            fields.append({
                "name": clean_name(m.group(3)),
                "type": decode_type(is_vector, m.group(2)),
                "donor_off": parse_offset(m.group(4)),
            })
        # Attach retail memory offsets when field counts agree.
        rfields = sorted(retail.get(name, {}).get("fields", []),
                         key=lambda f: f["off"])
        dfields_by_off = sorted(range(len(fields)),
                                key=lambda i: fields[i]["donor_off"])
        counts_match = len(rfields) == len(fields) and len(fields) > 0
        if counts_match:
            for rank, idx in enumerate(dfields_by_off):
                fields[idx]["retail_off"] = rfields[rank]["off"]
            with_offsets += 1
        for f in fields:
            total_fields += 1
            if f["name"]:
                named_fields += 1
        schema[name] = {"addr": addr, "retail_offsets": counts_match,
                        "fields": fields}

    OUT.write_text(json.dumps(schema, indent=1), encoding="utf-8")
    if FF_COPY.parent.exists():
        FF_COPY.write_text(json.dumps(schema, indent=1), encoding="utf-8")

    print(f"defs: {len(schema)}  fields: {total_fields}  named: {named_fields}  "
          f"with retail offsets: {with_offsets}")
    chest = schema.get("CChestDef", {})
    print("CChestDef (game.bin field order):")
    for f in chest.get("fields", []):
        ro = f.get("retail_off")
        print(f"  {f['type']:12} {f['name']}"
              + (f"  @retail +0x{ro:x}" if ro is not None else ""))


if __name__ == "__main__":
    main()
