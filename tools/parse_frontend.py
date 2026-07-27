#!/usr/bin/env python3
"""Frontend compiled-defs parser: data\\CompiledDefs\\frontend.bin -> UI widget tree.

Container = the SAME CompiledDefs format as game.bin/script.bin (layout from
FableForge libs/forgecore/include/forge/bin.hpp, cross-checked vs SilverChest):
  names.bin:    20-byte header (u32 magic @4, PC = 0xA8E36C34; s32 count @8),
                then count x { u32 nameCrc, ASCIIZ name }. Name references are
                byte offsets of the name string relative to END of header.
  frontend.bin: 9-byte header (byte[1] == 0xAA means Xbox build),
                u32 entryCount @9,
                main table @13 = entryCount x { s32 defNameOffset,
                    s32 nameOffset, s32 indexInDefinition },
                u32 chunkCount, chunkCount x { s32 firstEntry,
                    s32 compressedOffset }   (last pair = end sentinel),
                u32 (decompressed tail size hint), then zlib chunk stream.
                Each inflated chunk (<=64 KiB) begins with u16 payload
                offsets, one per entry in the chunk.
Payload field encoding (tag = CRC32 poly 0xEDB88320 seed 0 no-invert of field
name) is decode_game_bin_field.py's; schema = FableForge def_schema.json.

Usage:
  python tools/parse_frontend.py [<game-root>] [--schema def_schema.json]
         [--bin frontend.bin|game.bin|script.bin]
         [list | tree | entry <name-or-index> | survey
          | patch <out-dir> <entry-name> <FieldName> <hex-value>]
Default game root: the retail install; default mode: tree.
`patch` writes rebuilt names.bin+<bin> into <out-dir> (never the install);
the write path mirrors FableForge bin.cpp File::save (32 KiB inflated chunks,
(entryCount,totalCompressed) sentinel + trailing u32).
"""
import json
import os
import struct
import sys
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from decode_game_bin_field import field_tag  # noqa: E402

DEFAULT_ROOT = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters"
DEFAULT_SCHEMA = r"D:\Code\FableForge\docs\re_reference\def_schema.json"

# frontend.bin definition-type name -> def_schema type with the field list.
SCHEMA_ALIAS = {
    "UI": "CUIDef",
    "FRONT_END": "CFrontEndDef",
    "UI_MISC_THINGS_DEF": "CUIMiscThingsDef",
    "UI_ICONS_DEF": "CUIIconsDef",
    "CONTROL_SCHEME": "CControlsDef",
    "ENGINE": "CEngineDef",
    "ENGINE_VIDEO_OPTIONS": "CEngineVideoOptionsDef",
    "CONFIG_OPTIONS_DEFAULTS_DEF": "CConfigOptionsDefaultsDef",
}


def read_names(path):
    b = open(path, "rb").read()
    magic, count = struct.unpack_from("<Ii", b, 4)
    base = 20
    names = {}  # offset (rel to end of header) -> name
    p = base
    for _ in range(count):
        p += 4  # crc
        e = b.index(b"\x00", p)
        names[p - base] = b[p:e].decode("latin1")
        p = e + 1
    return names, magic


def read_bin(bin_path, names):
    b = open(bin_path, "rb").read()
    is_xbox = b[1] == 0xAA
    (count,) = struct.unpack_from("<I", b, 9)
    p = 13
    table = []
    for _ in range(count):
        dno, no, idx = struct.unpack_from("<iii", b, p)
        p += 12
        table.append((dno, no, idx))
    (chunk_count,) = struct.unpack_from("<I", b, p)
    p += 4
    chunks = []
    for _ in range(chunk_count):
        first, coff = struct.unpack_from("<ii", b, p)
        p += 8
        chunks.append((first, coff))
    p += 4  # tail u32
    data_base = p
    entries = []
    for ci in range(len(chunks) - 1):  # last pair is sentinel
        first, coff = chunks[ci]
        nxt_first = chunks[ci + 1][0]
        raw = zlib.decompress(b[data_base + coff:])
        n = nxt_first - first
        offs = list(struct.unpack_from("<%dH" % n, raw, 0)) + [len(raw)]
        for j in range(n):
            gi = first + j
            dno, no, idx = table[gi]
            entries.append(dict(
                index=gi,
                definition=names.get(dno, "?%d" % dno),
                name=names.get(no, "") if no >= 0 else "",
                index_in_definition=idx,
                payload=raw[offs[j]:offs[j + 1]],
            ))
    return entries, is_xbox


def save_bin(bin_path, out_path, entries):
    """Rebuild the compiled-defs bin with (possibly modified) entry payloads.
    Mirrors FableForge bin.cpp File::save. Preserves the original 9-byte
    header and the original main table rows (names unchanged)."""
    src = open(bin_path, "rb").read()
    header = src[:9]
    (count,) = struct.unpack_from("<I", src, 9)
    assert count == len(entries), "entry count changed; not supported here"
    main_table = src[13:13 + count * 12]
    kmax = 32768
    chunks = []          # (first_entry, compressed_offset)
    blobs = []
    total = 0
    i = 0
    while i < len(entries):
        first = i
        payload = bytearray()
        offs = []
        while i < len(entries):
            p = entries[i]["payload"]
            if offs and (len(offs) + 1) * 2 + len(payload) + len(p) > kmax:
                break
            offs.append(len(payload))
            payload += p
            i += 1
        n = len(offs)
        raw = struct.pack("<%dH" % n, *[n * 2 + o for o in offs]) + payload
        comp = zlib.compress(bytes(raw))
        chunks.append((first, total))
        total += len(comp)
        blobs.append(comp)
    out = bytearray(header)
    out += struct.pack("<I", count)
    out += main_table
    out += struct.pack("<I", len(chunks) + 1)
    for first, coff in chunks:
        out += struct.pack("<ii", first, coff)
    out += struct.pack("<II", count, total)   # end sentinel
    out += struct.pack("<I", total)           # trailing u32
    for blob in blobs:
        out += blob
    open(out_path, "wb").write(bytes(out))


def patch_field(payload, fields, field_name, new_value):
    """Return payload with `field_name`'s value bytes replaced."""
    named = [f["name"] for f in fields if f.get("name")]
    if field_name not in named:
        raise SystemExit("field not in schema: %s" % field_name)
    tag = struct.pack("<I", field_tag(field_name))
    at = payload.find(tag)
    if at < 0:
        raise SystemExit("field tag not present in payload: %s" % field_name)
    vstart = at + 4
    vend = len(payload)
    idx = named.index(field_name)
    for nxt in named[idx + 1:]:
        j = payload.find(struct.pack("<I", field_tag(nxt)), vstart)
        if j >= 0:
            vend = j
            break
    return payload[:vstart] + new_value + payload[vend:]


def decode_ui_states(raw, schema):
    """Decode the tagged CUIStateDef vector used by frontend UI widgets.

    The schema names this serialized type Vector_CUIStateDef_NUISystem even
    though the element schema is simply CUIStateDef.  Keeping this decoder
    here makes the retail position/scale/colour data inspectable instead of
    returning one large opaque hex string.
    """
    if len(raw) < 4 or "CUIStateDef" not in schema:
        return None
    count = struct.unpack_from("<I", raw)[0]
    pos = 4
    states = []
    for _ in range(count):
        state = {}
        for field in schema["CUIStateDef"]["fields"]:
            name = field["name"]
            typ = field["type"]
            if pos + 4 > len(raw):
                return None
            tag = struct.unpack_from("<I", raw, pos)[0]
            if tag != field_tag(name):
                return None
            pos += 4
            if typ == "bool":
                if pos + 1 > len(raw):
                    return None
                state[name] = bool(raw[pos])
                pos += 1
            elif typ == "Vector_uint32":
                if pos + 4 > len(raw):
                    return None
                item_count = struct.unpack_from("<I", raw, pos)[0]
                pos += 4
                size = item_count * 4
                if pos + size > len(raw):
                    return None
                state[name] = list(struct.unpack_from(
                    "<%dI" % item_count, raw, pos))
                pos += size
            elif typ == "float":
                if pos + 4 > len(raw):
                    return None
                state[name] = struct.unpack_from("<f", raw, pos)[0]
                pos += 4
            elif (
                typ in ("int32", "CDefIndex") or
                typ == "uint32" or
                typ.startswith(("enum", "W4", "E"))
            ):
                if pos + 4 > len(raw):
                    return None
                state[name] = struct.unpack_from("<I", raw, pos)[0]
                pos += 4
            else:
                return None
        states.append(state)
    return states if pos == len(raw) else None


def fval(typ, raw, schema=None, names=None):
    """Human value for a fixed-size field byte string."""
    if raw is None:
        return "<undecoded>"
    if typ == "bool":
        return bool(raw[0])
    if typ in ("int32", "CDefIndex"):
        return struct.unpack("<i", raw)[0]
    if typ == "uint32" or typ.startswith(("enum", "W4", "E")):
        return struct.unpack("<I", raw)[0] if len(raw) == 4 else raw.hex()
    if typ == "float":
        return struct.unpack("<f", raw)[0]
    if typ == "CWideString":
        return raw[:-2].decode("utf-16-le", "replace")
    if typ == "CCharString":
        return raw[:-1].decode("latin1", "replace")
    if typ == "CDefString":
        # CompiledDefs serializes a definition string as a names.bin-relative
        # u32 offset, not as an inline ASCIIZ string.  The old inline-string
        # interpretation happened to make offset 0x000000e0 look plausible,
        # but produced mojibake for the other font names.
        if len(raw) == 4:
            offset = struct.unpack("<I", raw)[0]
            if names is not None and offset in names:
                return names[offset]
            return "@names+0x%08X" % offset
        return raw.hex()
    if typ == "Vector_int32":
        n = struct.unpack_from("<I", raw)[0]
        return list(struct.unpack_from("<%di" % n, raw, 4))
    if typ == "Vector_uint32":
        n = struct.unpack_from("<I", raw)[0]
        return list(struct.unpack_from("<%dI" % n, raw, 4))
    if typ.startswith("Vector_CUIStateDef") and schema is not None:
        decoded = decode_ui_states(raw, schema)
        if decoded is not None:
            return decoded
    return raw.hex()


def decode_tagged(payload, fields):
    """Tag-delimited decode (mirrors FableForge defdecode.cpp): each named
    field's value runs from just after its 4-byte tag to the next named
    field's tag (or end of payload). Returns ({name: raw_bytes}, leftover)."""
    out = {}
    pos = None
    for f in fields:
        if f.get("name"):
            pos = payload.find(struct.pack("<I", field_tag(f["name"])))
            break
    if pos is None or pos < 0:
        return out, len(payload)
    named = [f for f in fields if f.get("name")]
    for i, f in enumerate(named):
        tag = struct.pack("<I", field_tag(f["name"]))
        at = payload.find(tag, pos)
        if at < 0:
            continue
        vstart = at + 4
        vend = len(payload)
        if i + 1 < len(named):
            nxt = payload.find(
                struct.pack("<I", field_tag(named[i + 1]["name"])), vstart)
            if nxt >= 0:
                vend = nxt
        out[f["name"]] = (f["type"], payload[vstart:vend])
        pos = vend
    return out, len(payload) - pos


def resolve_type(definition, schema):
    """Definition string -> def_schema type (mirrors forge resolveType)."""
    stype = SCHEMA_ALIAS.get(definition, definition)
    if stype in schema:
        return stype
    camel = "C" + "".join(w.capitalize() for w in definition.split("_")) + "Def"
    return camel if camel in schema else None


def decode_entry(entry, schema):
    stype = resolve_type(entry["definition"], schema)
    if stype is None:
        return None
    raw, leftover = decode_tagged(entry["payload"], schema[stype]["fields"])
    return {
        nm: fval(typ, v, schema, entry.get("_names"))
        for nm, (typ, v) in raw.items()
    }, leftover


def load_all(root, schema_path, bin_name="frontend.bin"):
    schema = json.load(open(schema_path))
    names, _ = read_names(
        os.path.join(root, "data", "CompiledDefs", "names.bin"))
    entries, is_xbox = read_bin(
        os.path.join(root, "data", "CompiledDefs", bin_name), names)
    for entry in entries:
        entry["_names"] = names
    return entries, schema, is_xbox


def cmd_list(entries):
    for e in entries:
        print("%4d  %-28s %6d bytes  %s" % (
            e["index"], e["definition"], len(e["payload"]), e["name"]))


def cmd_entry(entries, schema, key):
    for e in entries:
        if e["name"] == key or str(e["index"]) == key:
            print("entry %d: %s / %s (%d bytes)" % (
                e["index"], e["definition"], e["name"], len(e["payload"])))
            dec = decode_entry(e, schema)
            if dec is None:
                print("  <no schema>")
                return
            vals, leftover = dec
            for k, v in vals.items():
                print("  %-32s %r" % (k, v))
            print("  leftover=%d" % leftover)
            return
    print("not found: %s" % key)


def cmd_tree(entries, schema):
    """Print the UI widget hierarchy from Children vectors (entry indices)."""
    dec = {}
    for e in entries:
        if e["definition"] != "UI":
            continue
        d = decode_entry(e, schema)
        if d:
            dec[e["index"]] = d[0]
    child_of = {}
    for i, v in dec.items():
        for c in v.get("Children", []):
            child_of[c] = i
    roots = [i for i in dec if i not in child_of]

    def label(i):
        e = entries[i]
        v = dec.get(i, {})
        bits = ["#%d" % i, e["name"] or "(anon)"]
        t = v.get("Type")
        if t is not None:
            bits.append("type=%s" % t)
        txt = v.get("TextValue") or ""
        if txt:
            bits.append("text=%r" % txt)
        if v.get("Width") or v.get("Height"):
            bits.append("%gx%g" % (v.get("Width", 0), v.get("Height", 0)))
        return "  ".join(bits)

    def walk(i, depth, seen):
        print("%s%s" % ("  " * depth, label(i)))
        if i in seen:
            print("%s^cycle" % ("  " * (depth + 1)))
            return
        seen = seen | {i}
        for c in dec.get(i, {}).get("Children", []):
            if c in dec:
                walk(c, depth + 1, seen)
            else:
                print("%s#%d <non-UI or missing>" % ("  " * (depth + 1), c))

    for r in sorted(roots):
        walk(r, 0, set())
        print()


def cmd_survey(entries, schema):
    """Coverage check: decode every entry, report leftover bytes."""
    ok = bad = noschema = 0
    for e in entries:
        d = decode_entry(e, schema)
        if d is None:
            noschema += 1
            continue
        _, leftover = d
        if leftover == 0:
            ok += 1
        else:
            bad += 1
            print("dirty: entry %d %s/%s leftover=%d" % (
                e["index"], e["definition"], e["name"], leftover))
    print("clean=%d dirty=%d no-schema=%d total=%d" % (
        ok, bad, noschema, len(entries)))


def cmd_patch(entries, schema, root, bin_name, out_dir, key, field, hexval):
    target = None
    for e in entries:
        if e["name"] == key or str(e["index"]) == key:
            target = e
            break
    if target is None:
        raise SystemExit("entry not found: %s" % key)
    stype = resolve_type(target["definition"], schema)
    if stype is None:
        raise SystemExit("no schema for %s" % target["definition"])
    new_value = bytes.fromhex(hexval.replace(" ", ""))
    old = target["payload"]
    target["payload"] = patch_field(old, schema[stype]["fields"], field,
                                    new_value)
    os.makedirs(out_dir, exist_ok=True)
    src_dir = os.path.join(root, "data", "CompiledDefs")
    save_bin(os.path.join(src_dir, bin_name),
             os.path.join(out_dir, bin_name), entries)
    with open(os.path.join(src_dir, "names.bin"), "rb") as f:
        open(os.path.join(out_dir, "names.bin"), "wb").write(f.read())
    print("patched %s.%s (%d -> %d payload bytes) -> %s" % (
        target["name"], field, len(old), len(target["payload"]), out_dir))


def main():
    args = sys.argv[1:]
    schema_path = DEFAULT_SCHEMA
    bin_name = "frontend.bin"
    if "--schema" in args:
        i = args.index("--schema")
        schema_path = args[i + 1]
        del args[i:i + 2]
    if "--bin" in args:
        i = args.index("--bin")
        bin_name = args[i + 1]
        del args[i:i + 2]
    root = args.pop(0) if args and os.path.isdir(args[0]) else DEFAULT_ROOT
    mode = args.pop(0) if args else "tree"
    entries, schema, is_xbox = load_all(root, schema_path, bin_name)
    if mode == "list":
        cmd_list(entries)
    elif mode == "entry":
        cmd_entry(entries, schema, args[0])
    elif mode == "survey":
        cmd_survey(entries, schema)
    elif mode == "patch":
        cmd_patch(entries, schema, root, bin_name, *args[:4])
    else:
        cmd_tree(entries, schema)


if __name__ == "__main__":
    main()
