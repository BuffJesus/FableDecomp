#!/usr/bin/env python
"""Decode game.bin/script.bin/frontend.bin compiled-def payloads into named fields.

THE game.bin FIELD-TAG HASH (cracked + corpus-validated 2026-07-19):
    tag(fieldName) = reflected CRC-32 (poly 0xEDB88320, the standard zlib table)
                     with seed = 0 and NO final inversion, over the ASCII field
                     name as-is (no null terminator), stored LITTLE-ENDIAN.
    This is NOT CCharString::GetCRC() (which seeds 0xFFFFFFFF) -- that difference
    is why every earlier attempt (crc32/fnv/djb2/GetCRC) failed to match.

PAYLOAD LAYOUT (per compiled-def entry):
    [variable prefix][field]*
      prefix  = untagged base-class data; observed lengths 3 ('01 00 01') for most
                types, 5 ('01 00 01 00 00') for some, larger for a few. Located
                empirically: the first named field's 4-byte CRC tag marks its end.
      field   = [4-byte CRC(name) little-endian tag][value]
    value sizes by def_schema type:
      int32/uint32/float/enum(W4Exxx)/CDefIndex/J = 4 bytes
      bool (_N)                                    = 1 byte
      CCharString                                  = null-terminated (empty = 0x00)
      Vector_<T>                                   = [4-byte count][count x element(T)]
      (struct/Map_* types decode field-by-field via their own sub-schema)

Serialization order == Transfer() CALL order in def_schema.json (NOT offset order).

Validation: 145/145 schema'd types present in game.bin match every field tag in
order (scratchpad validate2.py). CChestDef/CContainerRewardHeroDef/
CFireheartMinigameDef fully round-trip byte-for-byte with leftover 0.
"""
import sys, json

_TBL = [0] * 256
for _n in range(256):
    _c = _n
    for _ in range(8):
        _c = (_c >> 1) ^ 0xEDB88320 if _c & 1 else _c >> 1
    _TBL[_n] = _c


def field_tag(name):
    """game.bin field-name tag: reflected CRC-32, seed 0, no final xor."""
    c = 0
    for b in name.encode("latin1"):
        c = (c >> 8) ^ _TBL[(c ^ b) & 0xFF]
    return c & 0xFFFFFFFF


_FIXED4 = {"int32", "uint32", "float", "CDefIndex", "J", "CGameFlag", "CBookIndex"}


def _is_enum(t):
    return t.startswith("enum") or t.startswith("W4") or (t.startswith("E") and t[1:2].isupper())


def _read_value(buf, pos, typ, schema):
    if typ in _FIXED4 or _is_enum(typ):
        return pos + 4
    if typ == "bool":
        return pos + 1
    if typ in ("CCharString", "CDefString"):
        return buf.index(0, pos) + 1
    if typ.startswith("Vector_"):
        count = int.from_bytes(buf[pos:pos + 4], "little")
        pos += 4
        el = typ[len("Vector_"):].lstrip("V").rstrip("_")
        for _ in range(count):
            np = _read_value(buf, pos, el, schema)
            if np is None:
                return None
            pos = np
        return pos
    if typ in schema:  # nested struct: tagged sub-fields
        for f in schema[typ]["fields"]:
            pos += 4  # sub-field tag
            np = _read_value(buf, pos, f["type"], schema)
            if np is None:
                return None
            pos = np
        return pos
    return None  # unknown type -> caller resyncs


def find_prefix_len(buf, deftype, schema):
    """Locate the end of the untagged prefix = offset of first named field's tag."""
    for f in schema[deftype]["fields"]:
        if f.get("name"):
            return buf.find(field_tag(f["name"]).to_bytes(4, "little"))
    return 0


def decode(buf, deftype, schema):
    """Return list of (name, type, value_bytes) and leftover byte count."""
    fields = schema[deftype]["fields"]
    pos = find_prefix_len(buf, deftype, schema)
    if pos < 0:
        pos = 3
    out = []
    for f in fields:
        tag = int.from_bytes(buf[pos:pos + 4], "little")
        nm = f.get("name")
        if nm and tag != field_tag(nm):
            # resync: jump to this field's tag if present later
            j = buf.find(field_tag(nm).to_bytes(4, "little"), pos)
            if j >= 0:
                pos = j
        pos += 4
        vstart = pos
        np = _read_value(buf, pos, f["type"], schema)
        if np is None:
            out.append((nm, f["type"], None))
            break
        pos = np
        out.append((nm, f["type"], buf[vstart:pos]))
    return out, len(buf) - pos


if __name__ == "__main__":
    if len(sys.argv) < 4:
        sys.exit("usage: decode_game_bin_field.py <def_schema.json> <DefType> <payload-hex>")
    schema = json.load(open(sys.argv[1]))
    buf = bytes.fromhex(sys.argv[3].replace(" ", ""))
    fields, leftover = decode(buf, sys.argv[2], schema)
    for nm, typ, val in fields:
        print(f"  {str(nm):28} {typ:22} {val.hex() if val else '<undecoded>'}")
    print(f"  leftover={leftover}")
