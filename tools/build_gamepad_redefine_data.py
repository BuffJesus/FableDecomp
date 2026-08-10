#!/usr/bin/env python3
"""Build the DATA half of the gamepad-redefine retail patch (frontend.bin +
names.bin), per docs/GAMEPAD_REDEFINE_PATCH.md section 4b.

Appends, using the crc0 + append rules from docs/DEF_LOAD_CONTRACT.md:
  1. clone options row #344 UI_OPTIONS_BUTTON_REDEFINE_KEYS -> ..._GAMEPAD,
     patch Action 283 -> NEW_ACTION (default 284).
  2. clone redefine screen #238 UI_FRONTEND_SCREEN_REDEFINE_KEYS_PC -> ..._GAMEPAD.
  3. append the new row's global index to options list #219
     UI_FRONTEND_LIST_OPTIONS_SUB_MENU Children [347,350,273,344] -> +new.

Writes rebuilt names.bin + frontend.bin to <out-dir> (NEVER the install) and
round-trip re-parses to verify. In-game validation (boot retail to the options
menu) is the final gate and needs the CODE detour (Action case + Init2 bind).

Usage: python tools/build_gamepad_redefine_data.py <out-dir> [new_action_id]
"""
import os, sys, struct, zlib
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_frontend as pf
from parse_frontend import read_names, read_bin, patch_field, resolve_type

ROOT = pf.DEFAULT_ROOT
SRC = os.path.join(ROOT, "data", "CompiledDefs")

def crc0(s):
    """Reflected CRC-32 poly 0xEDB88320, seed 0, NO final inversion (crc0)."""
    crc = 0
    for ch in s.encode("latin1"):
        crc ^= ch
        for _ in range(8):
            crc = (crc >> 1) ^ (0xEDB88320 if (crc & 1) else 0)
    return crc & 0xFFFFFFFF

def read_names_raw(path):
    b = bytearray(open(path, "rb").read())
    version, magic, count, tablesize, resv = struct.unpack_from("<IIIII", b, 0)
    # name text offset (rel +20) -> name  AND  name -> offset
    off2name = {}; name2off = {}
    p = 20
    for _ in range(count):
        crc, = struct.unpack_from("<I", b, p); p += 4
        e = b.index(b"\x00", p)
        nm = b[p:e].decode("latin1")
        off2name[p - 20] = nm; name2off[nm] = p - 20
        p = e + 1
    return b, dict(version=version, magic=magic, count=count,
                   tablesize=tablesize, resv=resv), off2name, name2off, p

def append_name(recs, name):
    """recs = bytearray of the name-record region (after 20-byte header)."""
    off = len(recs)  # text will sit at header+off+4
    recs += struct.pack("<I", crc0(name)) + name.encode("latin1") + b"\x00"
    return off + 4  # stored ref = position of TEXT relative to +20

def read_table(bin_path):
    b = open(bin_path, "rb").read()
    (count,) = struct.unpack_from("<I", b, 9)
    rows = [struct.unpack_from("<iii", b, 13 + i * 12) for i in range(count)]
    return b[:9], rows

def save_bin_append(header9, rows, payloads, out_path):
    """rows = list of (dno,no,idx); payloads = list of bytes (same length)."""
    count = len(rows)
    out = bytearray(header9)
    out += struct.pack("<I", count)
    for dno, no, idx in rows:
        out += struct.pack("<iii", dno, no, idx)
    kmax = 32768
    chunks = []; blobs = []; total = 0; i = 0
    while i < count:
        first = i; payload = bytearray(); offs = []
        while i < count:
            p = payloads[i]
            if offs and (len(offs) + 1) * 2 + len(payload) + len(p) > kmax:
                break
            offs.append(len(payload)); payload += p; i += 1
        n = len(offs)
        raw = struct.pack("<%dH" % n, *[n * 2 + o for o in offs]) + payload
        comp = zlib.compress(bytes(raw))
        chunks.append((first, total)); total += len(comp); blobs.append(comp)
    out += struct.pack("<I", len(chunks) + 1)
    for first, coff in chunks:
        out += struct.pack("<ii", first, coff)
    out += struct.pack("<II", count, total)
    out += struct.pack("<I", total)
    for blob in blobs:
        out += blob
    open(out_path, "wb").write(bytes(out))

def set_children(payload, new_list):
    """Replace the Vector_int32 Children field with exactly new_list."""
    tag = struct.pack("<I", pf.field_tag("Children"))
    at = payload.find(tag)
    assert at >= 0, "Children tag not found"
    vstart = at + 4
    (n,) = struct.unpack_from("<I", payload, vstart)
    arr_end = vstart + 4 + n * 4
    newbytes = struct.pack("<I", len(new_list)) + struct.pack(
        "<%di" % len(new_list), *new_list)
    return payload[:vstart] + newbytes + payload[arr_end:]

def edit_children(payload, new_index):
    """Append new_index to the Vector_int32 Children field of a CUIDef payload."""
    tag = struct.pack("<I", pf.field_tag("Children"))
    at = payload.find(tag)
    assert at >= 0, "Children tag not found"
    vstart = at + 4
    (n,) = struct.unpack_from("<I", payload, vstart)
    arr_end = vstart + 4 + n * 4
    old = list(struct.unpack_from("<%di" % n, payload, vstart + 4))
    new = old + [new_index]
    newbytes = struct.pack("<I", len(new)) + struct.pack("<%di" % len(new), *new)
    return payload[:vstart] + newbytes + payload[arr_end:], old, new

def main():
    if len(sys.argv) < 2:
        raise SystemExit(__doc__)
    out_dir = sys.argv[1]
    new_action = int(sys.argv[2]) if len(sys.argv) > 2 else 284
    os.makedirs(out_dir, exist_ok=True)
    import json
    schema = json.load(open(pf.DEFAULT_SCHEMA))
    names, _magic = read_names(os.path.join(SRC, "names.bin"))
    entries, is_xbox = read_bin(os.path.join(SRC, "frontend.bin"), names)
    header9, rows = read_table(os.path.join(SRC, "frontend.bin"))
    nb, nhdr, off2name, name2off, recs_start = read_names_raw(
        os.path.join(SRC, "names.bin"))
    recs = bytearray(nb[20:])

    by_idx = {e["index"]: e for e in entries}
    ROW344 = by_idx[344]; SCREEN238 = by_idx[238]; LIST219 = by_idx[219]
    BTN345 = by_idx[345]; TXT346 = by_idx[346]   # label chain: row->#345->#346(text)
    dno_ui = rows[344][0]           # "UI" class name offset (shared)
    ui_indices = [r[2] for r in rows if r[0] == dno_ui]
    next_ui_idx = max(ui_indices) + 1
    stype = resolve_type("UI", schema)
    gamepad_label = os.environ.get("GAMEPAD_LABEL", "Redefine Keys (Gamepad)")

    # 1. new names (row + screen get names; label-chain clones are anonymous sub-defs)
    off_row = append_name(recs, "UI_OPTIONS_BUTTON_REDEFINE_KEYS_GAMEPAD")
    off_scr = append_name(recs, "UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD")

    # global indices of the 4 appended entries
    new_row_gi = len(rows); new_scr_gi = len(rows) + 1
    new_txt_gi = len(rows) + 2; new_btn_gi = len(rows) + 3

    # 2. clone the label text child #346 -> new_txt_gi, set its CWideString TextValue
    #    to the distinct gamepad label (retail stores the tag string inline in UTF-16).
    txt_payload = patch_field(
        TXT346["payload"], schema[stype]["fields"], "TextValue",
        gamepad_label.encode("utf-16-le") + b"\x00\x00")
    # 3. clone the redefine button #345 -> new_btn_gi, repoint its Children
    #    [346, 73] -> [new_txt_gi, 73] (keep the shared mouse-area button #73).
    btn_dec, _ = pf.decode_tagged(BTN345["payload"], schema[stype]["fields"])
    btn_children = pf.fval(*btn_dec["Children"])            # [346, 73]
    btn_children = [new_txt_gi if c == 346 else c for c in btn_children]
    btn_payload = set_children(BTN345["payload"], btn_children)
    # 4. clone row #344 -> new_row_gi: Action -> new_action, Children [345] -> [new_btn_gi]
    row_payload = patch_field(ROW344["payload"], schema[stype]["fields"],
                              "Action", struct.pack("<i", new_action))
    row_payload = set_children(row_payload, [new_btn_gi])

    # 5. edit list #219 Children -> append the new row global index
    list_payload, old_children, new_children = edit_children(
        LIST219["payload"], new_row_gi)

    # assemble
    payloads = [e["payload"] for e in entries]
    payloads[219] = list_payload
    payloads.append(row_payload)           # new_row_gi
    payloads.append(SCREEN238["payload"])  # new_scr_gi
    payloads.append(txt_payload)           # new_txt_gi
    payloads.append(btn_payload)           # new_btn_gi
    new_rows = list(rows)
    new_rows.append((dno_ui, off_row, next_ui_idx))
    new_rows.append((dno_ui, off_scr, next_ui_idx + 1))
    new_rows.append((dno_ui, -1, next_ui_idx + 2))   # anonymous text clone
    new_rows.append((dno_ui, -1, next_ui_idx + 3))   # anonymous button clone

    # write names.bin (append) + frontend.bin
    nout = bytearray(nb[:20])
    struct.pack_into("<I", nout, 8, nhdr["count"] + 2)
    struct.pack_into("<I", nout, 12, len(recs) + 4)   # tableSize convention
    nout += recs
    open(os.path.join(out_dir, "names.bin"), "wb").write(bytes(nout))
    save_bin_append(header9, new_rows, payloads,
                    os.path.join(out_dir, "frontend.bin"))

    print("APPEND: row gi=%d (Action %d), screen gi=%d, text gi=%d (%r), button gi=%d"
          % (new_row_gi, new_action, new_scr_gi, new_txt_gi, gamepad_label, new_btn_gi))
    print("  #219 Children %s -> %s ; row.Children -> [%d] ; button.Children -> %s"
          % (old_children, new_children, new_btn_gi, btn_children))
    print("  names +2 (%d -> %d)" % (nhdr["count"], nhdr["count"] + 2))

    # ---- round-trip verify ----
    n2, _ = read_names(os.path.join(out_dir, "names.bin"))
    e2, _ = read_bin(os.path.join(out_dir, "frontend.bin"), n2)
    b2 = {e["index"]: e for e in e2}
    assert len(e2) == len(entries) + 4, "entry count wrong: %d" % len(e2)
    # verify the label clone carries the gamepad text
    tdec, _ = pf.decode_tagged(b2[new_txt_gi]["payload"], schema[stype]["fields"])
    tv = pf.fval(*tdec["TextValue"])
    assert tv == gamepad_label, "label mismatch: %r" % tv
    row = b2[new_row_gi]; scr = b2[new_scr_gi]
    assert row["name"] == "UI_OPTIONS_BUTTON_REDEFINE_KEYS_GAMEPAD", row["name"]
    assert scr["name"] == "UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD", scr["name"]
    row = b2[new_row_gi]; scr = b2[new_scr_gi]
    assert row["name"] == "UI_OPTIONS_BUTTON_REDEFINE_KEYS_GAMEPAD", row["name"]
    assert scr["name"] == "UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD", scr["name"]
    # decode #219 Children + new row Action  (decode_tagged -> {nm:(typ,bytes)})
    dec219, _ = pf.decode_tagged(b2[219]["payload"], schema[stype]["fields"])
    ch = pf.fval(*dec219["Children"])
    decrow, _ = pf.decode_tagged(row["payload"], schema[stype]["fields"])
    act = pf.fval(*decrow["Action"])
    assert act == new_action, "Action mismatch: %s" % act
    assert new_row_gi in ch, "new row index not in #219 Children"
    # full-parse integrity: every ORIGINAL entry byte-identical except #219, and
    # every patched entry decodes cleanly under the schema.
    changed = [e["index"] for e in entries
               if b2[e["index"]]["payload"] != e["payload"]
               or b2[e["index"]]["name"] != e["name"]]
    assert changed == [219], "unexpected collateral changes: %s" % changed
    bad = 0
    for e in e2:
        st2 = resolve_type(e["definition"], schema)
        if st2 and st2 in schema:
            try:
                e["_names"] = n2; pf.decode_entry(e, schema)
            except Exception:
                bad += 1
    assert bad == 0, "%d patched entries failed to decode" % bad
    print("VERIFY round-trip OK: #219 Children=%s, new-row Action=%s, crc0(row)=0x%08X"
          % (ch, act, crc0("UI_OPTIONS_BUTTON_REDEFINE_KEYS_GAMEPAD")))
    print("VERIFY full-parse OK: %d->%d entries, only #219 changed, all decode, "
          "names crc0 clean" % (len(entries), len(e2)))

if __name__ == "__main__":
    main()
