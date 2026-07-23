#!/usr/bin/env python3
"""
save_edit.py - Fable: The Lost Chapters runtime save (.sav / "FableSave!") read->edit->write pipeline.

Pure Python (stdlib only: struct, zlib). No Ghidra, no third-party deps.

Format (see docs/SAVEGAME_FORMAT.md + docs/SAVE_WRITER.md):

  File:
    0x00  "FableSave!"                 10 bytes ASCII magic (no NUL)
    0x0A  00 00                        2 pad
    0x0C  u32 total_data_len           == offset of the trailer (end of compressed data)
                                          (this is the field SAVEGAME_FORMAT.md called the
                                           "signature/version" -- it is NOT a signature; it is a length)
    0x10  u32 chunk0_uncompressed_len  == inflated HEADER-section size
    0x14  u32 chunk1_uncompressed_len  == inflated main/ENTITIES payload size
    0x18  u32 chunk0_compressed_len    == byte length of the first zlib blob
    0x1C  chunk0 zlib stream (78 DA), length chunk0_compressed_len
          u32 chunk1_compressed_len    (framing dword)
          chunk1 zlib stream (78 DA)
          u32 trailer_crc              seed-0 CRC-32 over file[0:trailer_pos]  <-- the real "signature"
          zero padding to 0x4B000 (307200) bytes

  Inflated persist stream (chunk0 || chunk1) = CPersistContext binary mode:
    section: [name ASCII NUL-terminated][u32 sectionByteLen][field]...
    field:   [u32 tag][value]   tag = CCRC::Calc(0, name, len)  <-- SEED 0 (empirically proven;
             see SAVEGAME_FORMAT.md CORRECTION. get_crc() below keeps the old 0xFFFFFFFF default
             for back-compat; use getcrc0() for field tags.)

CRC family (both seeds share the standard zlib/PKZIP reflected table, poly 0xEDB88320, NO final inversion):
    save field tag  : CCRC::Calc(0, name, len)              <-- seed 0, CONFIRMED on-disk
    file trailer sig: CCRC::Calc(0, file[0:trailer_pos])    <-- seed 0, CONFIRMED across 5 real saves

Entity-graph (SAVED_ENTITIES) hero-editing layer at the bottom of this file:
    hero_report / set_hero_stat / set_item_qty / add_item  (see docs/SAVE_ENTITY_GRAPH.md)
"""

import struct
import zlib

MAGIC = b"FableSave!"
SLAB_SIZE = 0x4B000  # 307200, fixed preallocated file size
SAVE_SEED = 0xFFFFFFFF   # CCharString::GetCRC (save-stream field tags)
SIG_SEED = 0x00000000    # file trailer signature seed (CONFIRMED)


# ---------------------------------------------------------------------------
# ENGINE-EXACT CELL DEFLATE (prefer classic zlib) — corrected rationale, 3rd pass.
#
# Fable compresses every SAVED_ENTITIES cell with CLASSIC zlib (v1.2.x) LEVEL 1
# via CCompressorZlib::InternalCompress @0x00a3aa40 (deflateInit level 1, memLevel
# 8 -> header 78 01). Under stock-zlib CPython, zlib.compress(inflated, 1)
# reproduces the engine's hero cell BYTE-FOR-BYTE (proven: identity rebuild of an
# adult save is byte-identical to the original on disk). We therefore PREFER a
# classic-zlib runtime so edited cells match the engine's own bytes as closely as
# possible.
#
# IMPORTANT correction to the earlier (2nd-pass) note: the retail LOAD path does
# NOT require byte-exactness. It was decompiled end-to-end this pass
# (CWorld::LoadGameStateInternal 0x4A21F0 -> CThingManager::LoadGameState 0x5253F0
# -> FUN_00520670 -> CCompressorZlib::InternalDecompress 0xA3AB00): the entity
# restore is purely LENGTH-FRAMED, CPersistContext::CheckCRC @0x404500 is a NO-OP
# on read (reads+discards 4 bytes), and the per-cell inflate is a persistent
# z_stream doing inflate(Z_FINISH) with avail_out=ulen that accepts ANY valid
# zlib stream regardless of level/flavor. So a zlib-ng cell (byte-different, ~+10%
# larger, still valid, inflates to the same ulen) is architecturally EXPECTED to
# load fine too. The confirmed-"childhood" saves were confounded (a zlib-ng save
# that also happened to be built on a different base, and a level-9 edit of a
# DIFFERENT base save with a different HEADER) — none was a clean engine-shaped-L1
# gold edit. See work/save_childhood_fix/FINDINGS.txt (3rd pass).
#
# Policy: WARN (do not hard-refuse) under a non-classic zlib. The output is still
# load-valid; it just won't be byte-identical to the engine writer. For maximal
# fidelity (and to keep total_data_len minimal) run under `py -3.13` / `py -3.12`.
_ZLIB_RUNTIME = getattr(zlib, "ZLIB_RUNTIME_VERSION", zlib.ZLIB_VERSION)
_ZLIB_IS_CLASSIC = "zlib-ng" not in _ZLIB_RUNTIME.lower()


def _require_classic_zlib(context):
    """Warn (not refuse) when re-compressing a cell under a non-classic zlib.

    A cell re-deflated by zlib-ng inflates fine but is a byte-different (larger)
    stream than the engine's classic-zlib output. Decompilation of the retail load
    path shows this is load-valid (length-framed, no content checksum, flavor-
    tolerant inflate), so we do NOT refuse — but for engine-exact bytes and minimal
    file growth, prefer a stock-zlib CPython (`py -3.13` / `py -3.12`).
    """
    if not _ZLIB_IS_CLASSIC:
        import sys
        sys.stderr.write(
            "save_edit: WARNING rebuilding %s under a NON-CLASSIC zlib "
            "(runtime=%r). The cell will be a byte-different, ~10%% larger (but "
            "valid) zlib stream than Fable's classic-zlib writer. The retail load "
            "path is length-framed and flavor-tolerant, so this is load-valid, but "
            "for engine-exact bytes and minimal total_data_len re-run under a "
            "stock-zlib CPython (e.g. `py -3.13` or `py -3.12`).\n"
            % (context, _ZLIB_RUNTIME))

# --- reflected CRC-32 table, poly 0xEDB88320 (standard zlib table; matches game's donor 0x0449BE30) ---
_CRC_TABLE = []
for _i in range(256):
    _c = _i
    for _ in range(8):
        _c = (_c >> 1) ^ 0xEDB88320 if (_c & 1) else (_c >> 1)
    _CRC_TABLE.append(_c)


def ccrc_calc(data, seed):
    """CCRC::Calc(seed, data, len) - reflected CRC-32, no final inversion. Caller supplies seed."""
    crc = seed & 0xFFFFFFFF
    for b in data:
        crc = (crc >> 8) ^ _CRC_TABLE[(crc ^ b) & 0xFF]
    return crc & 0xFFFFFFFF


def get_crc(name):
    """CCharString::GetCRC() - save-stream field tag. Hash over raw ASCII name (no NUL), seed 0xFFFFFFFF."""
    if isinstance(name, str):
        name = name.encode("ascii")
    return ccrc_calc(name, SAVE_SEED)


def sig_crc(data):
    """File trailer signature: CCRC::Calc(0, data). data == file bytes [0:trailer_pos]."""
    return ccrc_calc(data, SIG_SEED)


def getcrc0(name):
    """Persist-stream FIELD TAG hash: CCRC::Calc(0, name, len) — seed 0 (the confirmed on-disk seed)."""
    if isinstance(name, str):
        name = name.encode("ascii")
    return ccrc_calc(name, 0)


# ============================================================================
# Container (file) level: read -> chunks/payload -> rebuild
# ============================================================================

class SaveFile:
    """A parsed FableSave! container. Holds the inflated persist payload and chunk split point."""

    def __init__(self, sig_field, chunk0_ulen, chunk1_ulen, payload,
                 orig_chunk0_clen=None, orig_chunk1_clen=None, orig_trailer=None,
                 orig_total_len=None, orig_bytes=None):
        self.sig_field = sig_field            # the +0x0C dword as read (== total_data_len)
        self.chunk0_ulen = chunk0_ulen        # inflated len of chunk0 (HEADER section)
        self.chunk1_ulen = chunk1_ulen        # inflated len of chunk1 (main/ENTITIES)
        self.payload = payload                # inflated chunk0 || chunk1
        self.orig_chunk0_clen = orig_chunk0_clen
        self.orig_chunk1_clen = orig_chunk1_clen
        self.orig_trailer = orig_trailer
        self.orig_total_len = orig_total_len
        self.orig_bytes = orig_bytes

    @property
    def chunk0(self):
        return self.payload[:self.chunk0_ulen]

    @property
    def chunk1(self):
        return self.payload[self.chunk0_ulen:]


def read_save(data):
    """Parse a FableSave! file (bytes) -> SaveFile. Verifies the trailer signature."""
    if data[:10] != MAGIC:
        raise ValueError("not a FableSave! file (bad magic)")
    sig_field, c0u, c1u, c0c = struct.unpack_from("<IIII", data, 0x0C)
    off = 0x1C
    b0 = data[off:off + c0c]
    off += c0c
    (c1c,) = struct.unpack_from("<I", data, off)
    off += 4
    b1 = data[off:off + c1c]
    off += c1c
    trailer_pos = off
    (trailer,) = struct.unpack_from("<I", data, trailer_pos)

    i0 = zlib.decompress(b0)
    i1 = zlib.decompress(b1)
    if len(i0) != c0u:
        raise ValueError("chunk0 inflated length mismatch: got %d want %d" % (len(i0), c0u))
    if len(i1) != c1u:
        raise ValueError("chunk1 inflated length mismatch: got %d want %d" % (len(i1), c1u))

    # Verify the +0x0C field is the total compressed-data length (offset to trailer).
    if sig_field != trailer_pos:
        raise ValueError("+0x0C field (%d) != trailer offset (%d)" % (sig_field, trailer_pos))

    # Verify trailer signature = seed-0 CRC over file[0:trailer_pos].
    want = sig_crc(data[:trailer_pos])
    if want != trailer:
        raise ValueError("trailer signature mismatch: file=%08x computed=%08x" % (trailer, want))

    return SaveFile(sig_field, c0u, c1u, i0 + i1,
                    orig_chunk0_clen=c0c, orig_chunk1_clen=c1c, orig_trailer=trailer,
                    orig_total_len=trailer_pos, orig_bytes=data)


def build_save(chunk0_ulen, chunk1_ulen, payload):
    """Rebuild a valid FableSave! file (bytes, padded to SLAB_SIZE) from an inflated payload.

    Re-deflates each chunk (zlib level 9, 78 DA), fixes all header dwords, appends the
    recomputed seed-0 trailer signature, pads to 0x4B000. Returns the full 307200-byte file.
    """
    i0 = payload[:chunk0_ulen]
    i1 = payload[chunk0_ulen:]
    assert len(i1) == chunk1_ulen, "payload split does not match chunk lengths"

    b0 = _deflate(i0)
    b1 = _deflate(i1)

    body = bytearray()
    body += MAGIC
    body += b"\x00\x00"
    # +0x0C total_data_len is filled after we know trailer_pos; placeholder for now.
    hdr_total_pos = len(body)
    body += struct.pack("<I", 0)                # total_data_len (patched below)
    body += struct.pack("<I", chunk0_ulen)      # chunk0 uncompressed
    body += struct.pack("<I", chunk1_ulen)      # chunk1 uncompressed
    body += struct.pack("<I", len(b0))          # chunk0 compressed
    body += b0
    body += struct.pack("<I", len(b1))          # chunk1 compressed (framing dword)
    body += b1

    trailer_pos = len(body)
    struct.pack_into("<I", body, hdr_total_pos, trailer_pos)  # +0x0C == trailer offset

    trailer = sig_crc(bytes(body[:trailer_pos]))
    body += struct.pack("<I", trailer)

    if len(body) > SLAB_SIZE:
        raise ValueError("save payload exceeds 300KB slab (%d > %d)" % (len(body), SLAB_SIZE))
    body += b"\x00" * (SLAB_SIZE - len(body))
    return bytes(body)


def _deflate(data):
    """zlib deflate at best compression -> 78 DA stream (matches CCompressorZlib output framing)."""
    return zlib.compress(data, 9)


# ============================================================================
# Persist-stream level: walk sections -> [tag][value] fields
# ============================================================================

# Value type codes used by the editor for typed set/get. The on-disk stream carries NO
# type marker per field, so the caller must state the type when editing.
TYPE_U32 = "u32"      # 4-byte LE (also long/uint32/CIntelligentPointer handle)
TYPE_I32 = "i32"      # 4-byte LE signed
TYPE_F32 = "f32"      # 4-byte IEEE-754 LE
TYPE_BOOL = "bool"    # 1 byte
TYPE_STR = "str"      # NUL-terminated ASCII


class Field:
    """One [u32 tag][value] field located in the inflated stream, with byte extents."""
    __slots__ = ("tag", "section", "value_off", "value_end")

    def __init__(self, tag, section, value_off, value_end):
        self.tag = tag                 # u32 CRC tag
        self.section = section         # section name it lives in
        self.value_off = value_off     # byte offset in payload where value bytes start
        self.value_end = value_end     # byte offset where value bytes end (exclusive)


def iter_sections(payload):
    """Yield (name, sec_data_off, sec_len) for each top-level section in the inflated stream.

    A section is: [name ASCII NUL-terminated][u32 sectionByteLen][sectionByteLen bytes of fields].
    Sections are laid out back-to-back. Stops when a well-formed section name can no longer be read.
    """
    off = 0
    n = len(payload)
    while off < n:
        nul = payload.find(b"\x00", off)
        if nul < 0:
            break
        name = payload[off:nul]
        # A valid section name is short printable ASCII.
        if not name or not all(0x20 <= c < 0x7F for c in name):
            break
        size_off = nul + 1
        if size_off + 4 > n:
            break
        (sec_len,) = struct.unpack_from("<I", payload, size_off)
        data_off = size_off + 4
        if data_off + sec_len > n:
            # size implausible -> not a real section boundary; stop.
            break
        yield name.decode("ascii"), data_off, sec_len
        off = data_off + sec_len


def find_fields_by_tag(payload, tag, value_type):
    """Find all fields whose u32 tag == `tag`, decoding the value as value_type to learn its length.

    Because the stream has no per-field type marker and no field length, we scan within each
    section for the 4-byte tag and, on a match, consume exactly the bytes the given type occupies.
    Returns a list of Field. NOTE: this is a targeted scan; for a fully positional walk the caller
    must know the complete field-order schema (not required for generic set-field-by-tag edits).
    """
    tag_le = struct.pack("<I", tag)
    out = []
    for name, data_off, sec_len in iter_sections(payload):
        end = data_off + sec_len
        i = data_off
        while i + 4 <= end:
            if payload[i:i + 4] == tag_le:
                voff = i + 4
                try:
                    vend = _value_end(payload, voff, value_type, end)
                except ValueError:
                    i += 1
                    continue
                out.append(Field(tag, name, voff, vend))
                i = vend
            else:
                i += 1
    return out


def _value_end(payload, voff, value_type, limit):
    if value_type in (TYPE_U32, TYPE_I32, TYPE_F32):
        if voff + 4 > limit:
            raise ValueError("value truncated")
        return voff + 4
    if value_type == TYPE_BOOL:
        if voff + 1 > limit:
            raise ValueError("value truncated")
        return voff + 1
    if value_type == TYPE_STR:
        nul = payload.find(b"\x00", voff, limit)
        if nul < 0:
            raise ValueError("unterminated string")
        return nul + 1
    raise ValueError("unknown type %r" % value_type)


def decode_value(payload, field, value_type):
    b = payload[field.value_off:field.value_end]
    if value_type == TYPE_U32:
        return struct.unpack("<I", b)[0]
    if value_type == TYPE_I32:
        return struct.unpack("<i", b)[0]
    if value_type == TYPE_F32:
        return struct.unpack("<f", b)[0]
    if value_type == TYPE_BOOL:
        return b[0] != 0
    if value_type == TYPE_STR:
        return b[:-1].decode("latin-1")
    raise ValueError("unknown type")


def encode_value(value, value_type):
    if value_type == TYPE_U32:
        return struct.pack("<I", value & 0xFFFFFFFF)
    if value_type == TYPE_I32:
        return struct.pack("<i", value)
    if value_type == TYPE_F32:
        return struct.pack("<f", value)
    if value_type == TYPE_BOOL:
        return b"\x01" if value else b"\x00"
    if value_type == TYPE_STR:
        if isinstance(value, str):
            value = value.encode("latin-1")
        return value + b"\x00"
    raise ValueError("unknown type")


def set_field_by_tag(payload, tag, value, value_type, occurrence=0):
    """Return a new payload with the `occurrence`-th field matching `tag` set to `value`.

    For fixed-width types the section length is unchanged. For variable-width types (str),
    the enclosing section's u32 length dword is patched to keep the stream well-formed.
    """
    fields = find_fields_by_tag(payload, tag, value_type)
    if occurrence >= len(fields):
        raise KeyError("tag %08x occurrence %d not found (found %d)" % (tag, occurrence, len(fields)))
    f = fields[occurrence]
    new_val = encode_value(value, value_type)
    old_len = f.value_end - f.value_off
    delta = len(new_val) - old_len

    new_payload = bytearray(payload)
    new_payload[f.value_off:f.value_end] = new_val

    if delta != 0:
        # Patch the enclosing section length dword. Find the section header for f.section.
        for name, data_off, sec_len in iter_sections(payload):
            if name == f.section and data_off <= f.value_off < data_off + sec_len:
                len_dword_off = data_off - 4
                struct.pack_into("<I", new_payload, len_dword_off, sec_len + delta)
                break
    return bytes(new_payload)


# ============================================================================
# Entity-graph (SAVED_ENTITIES) hero-editing layer
# ============================================================================
# Layout (all CONFIRMED on real bytes across 6 saves / 81 cells — docs/SAVE_ENTITY_GRAPH.md):
#
#   chunk1 inflated stream contains:  "SAVED_ENTITIES\0" [u32 sectionLen] <section data>
#   section data = [u32 0][u32 N?][u32 0] head, then a run of CELL RECORDS (+ empty slots),
#   then a trailing UID->cellId index table (references cell IDs, not offsets — no patch needed).
#
#   Every NON-EMPTY cell record (the editable unit):
#       [u64 0]                      marker
#       [u64 recLen  = 29 + clen]    record length (id+clen+ulen+data+9 trailing zero bytes)
#       [u64 cellId]                 region/cell identifier (stable across edits)
#       [u64 clen]                   compressed byte length of the zlib cell
#       [u32 ulen]                   uncompressed (inflated) byte length
#       [clen bytes zlib stream]     78 xx — one region's entity records
#       [9 zero bytes]
#
#   On ANY cell edit you MUST patch recLen + clen (+ ulen if the inflated size changed),
#   the SAVED_ENTITIES [u32 sectionLen], and chunk1_ulen — then rebuild + re-sign the container.
#
#   Hero component grammar used below (CONFIRMED — docs/SAVE_HERO_STATS.md, SAVE_ENTITY_GRAPH.md):
#     component:  [class ASCII \0][u32 0][u8 0][u32 dataLen][dataLen bytes][u32 0]
#     CTCHeroStats fields: [u32 getcrc0(name)][4-byte value] (Money/Morality/RenownLevel i32,
#                          Age/Fatness f32)
#     CTCInventory* data:  [u32 getcrc0("InventoryItems")][u32 itemCount][u8 0]
#                          itemCount x record; record = [u32 nameLen][name chars, no NUL]
#                          [u32 quantity][u32 unkA][u8 ext] (+ variable extension iff ext != 0)

SAVED_ENTITIES = b"SAVED_ENTITIES\x00"
INV_LIST_TAG = 0x2B837863          # getcrc0("InventoryItems")
CELL_HDR = 36                      # bytes of descriptor before the zlib stream
CELL_TRAILER = 9                   # zero bytes after the zlib stream

HERO_STAT_FIELDS = {               # name -> struct fmt (all 4-byte values)
    "Money": "<i", "Morality": "<i", "RenownLevel": "<i",
    "Age": "<f", "Fatness": "<f",
}
INV_COMPONENTS = (b"CTCInventory", b"CTCInventoryClothing", b"CTCInventoryWeapons")


class Cell:
    __slots__ = ("desc_off", "data_off", "clen", "ulen", "cell_id", "inflated")

    def __init__(self, desc_off, data_off, clen, ulen, cell_id, inflated):
        self.desc_off = desc_off    # offset (in chunk1) of the [u64 0] marker
        self.data_off = data_off    # offset (in chunk1) of the zlib stream
        self.clen = clen
        self.ulen = ulen
        self.cell_id = cell_id
        self.inflated = inflated


def find_saved_entities(chunk1):
    """Return (data_off, section_len) of the SAVED_ENTITIES section inside inflated chunk1."""
    i = chunk1.find(SAVED_ENTITIES)
    if i < 0:
        raise ValueError("SAVED_ENTITIES section not found")
    (sec_len,) = struct.unpack_from("<I", chunk1, i + len(SAVED_ENTITIES))
    return i + len(SAVED_ENTITIES) + 4, sec_len


def find_cells(chunk1):
    """Locate every non-empty zlib cell in SAVED_ENTITIES and VERIFY its 36-byte descriptor.
    Returns list[Cell]. Raises if any descriptor invariant fails (do not edit blind)."""
    data_off, sec_len = find_saved_entities(chunk1)
    end = data_off + sec_len
    cells = []
    pos = data_off
    while pos < end - 1:
        if not (chunk1[pos] == 0x78 and chunk1[pos + 1] in (0x01, 0x5E, 0x9C, 0xDA)):
            pos += 1
            continue
        try:
            dc = zlib.decompressobj()
            out = dc.decompress(chunk1[pos:end])
            consumed = (end - pos) - len(dc.unused_data)
        except zlib.error:
            pos += 1
            continue
        marker, rec_len, cell_id, clen = struct.unpack_from("<QQQQ", chunk1, pos - CELL_HDR)
        (ulen,) = struct.unpack_from("<I", chunk1, pos - 4)
        if not (marker == 0 and rec_len == 29 + clen and clen == consumed and ulen == len(out)
                and chunk1[pos + consumed:pos + consumed + CELL_TRAILER] == b"\x00" * CELL_TRAILER):
            raise ValueError("cell descriptor invariant failed at chunk1 offset %d" % pos)
        cells.append(Cell(pos - CELL_HDR, pos, clen, ulen, cell_id, out))
        pos += consumed + CELL_TRAILER
    if not cells:
        raise ValueError("no SAVED_ENTITIES cells found")
    return cells


def hero_cell(cells):
    """The hero's cell is the (unique) one containing the CTCHeroStats component."""
    hits = [c for c in cells if b"CTCHeroStats\x00" in c.inflated]
    if len(hits) != 1:
        raise ValueError("expected exactly 1 hero cell, found %d" % len(hits))
    return hits[0]


def replace_cell(chunk1, cell, new_inflated):
    """Splice an edited inflated cell back into chunk1: re-deflate (classic zlib level 1, the
    engine's own cell format — CCompressorZlib::InternalCompress @0x00a3aa40 does
    `deflateInit(strm, 1, "1.2.1")` -> header 78 01), patch the record DESCRIPTOR
    (recLen/clen/ulen), and patch the SAVED_ENTITIES section length by the size delta so every
    downstream record stays correctly framed. Returns new chunk1 bytes.

    ★ RECORD FRAMING (CONFIRMED against retail load path + live crash, 5th pass 2026-07-20):
    the SAVED_ENTITIES section is [u32 0][u32 N][u32 0] head then a run of RECORDS the engine
    walks SEQUENTIALLY by advancing a byte pointer:
        record @p:  [u64 marker=0][u64 recLen][u64 cellId][u64 clen][u32 ulen][clen bytes zlib]
                    [1-byte trailer]
        recLen == 29 + clen   (= recLen_field 8 + cellId 8 + clen_field 8 + ulen 4 + clen + 1)
        NEXT record marker is at  p + 8 + recLen.
    The engine advances by recLen (NOT to an absolute offset, NOT by re-scanning for 78 01). So
    on a size change the cell must stay CONTIGUOUS with its neighbours: you patch recLen (=29+
    new_clen) and clen, write the new compressed stream in place, and the whole tail simply
    shifts by (new_clen - old_clen). You must ALSO patch the SAVED_ENTITIES [u32 sectionLen] and
    chunk1_ulen by the same delta. This keeps recLen-advance valid end-to-end.

    ✗ DO NOT pad the trailer to preserve absolute offsets. That was tried and it HARD-CRASHES
    the game: padding puts non-descriptor bytes where the engine — advancing by recLen — expects
    the next [u64 marker][u64 recLen]... descriptor, so it reads a bogus recLen/clen and
    dereferences garbage. (Live: the -1-shift build childhoods gracefully because its framing is
    valid; the padded build crashes because its framing is not.) The correct behaviour is to
    SHIFT, never pad.

    Other decompiled facts (load path 0x4A21F0 -> 0x5253F0 -> FUN_00520670 -> LoadSingleThing
    0x51fd80 -> hero gate CThingPlayerCreature::OnCreate 0x6a9ef0): the walk is length-framed,
    CPersistContext::CheckCRC @0x404500 is a NO-OP on read, each field is [u32 tag][value] read
    positionally, Money is a fixed-width 4-byte value invisible to every gate, and the per-cell
    inflate (0xa3ab00) is level/flavor tolerant (inflate Z_FINISH, avail_out=ulen). So the shift-
    correct rebuild is structurally valid; a stock-zlib-L1 re-deflate matches the engine writer
    byte-for-byte on unchanged content.

    Container chunks (build_save/_deflate) stay level 9 (retail writes those at level 9); only
    the inner CELL is level 1. Prefer a stock-zlib CPython (see _require_classic_zlib)."""
    _require_classic_zlib("SAVED_ENTITIES hero cell")
    comp = zlib.compress(new_inflated, 1)
    # Byte-integrity self-check: the re-deflated cell MUST inflate back to exactly what we
    # asked for (guards against any silent deflate corruption on exotic zlib builds).
    if zlib.decompress(comp) != new_inflated:
        raise RuntimeError("re-deflated cell does not round-trip — refusing to write")
    out = bytearray(chunk1)
    out[cell.data_off:cell.data_off + cell.clen] = comp
    struct.pack_into("<Q", out, cell.desc_off + 8, 29 + len(comp))    # recLen = 29 + clen
    struct.pack_into("<Q", out, cell.desc_off + 24, len(comp))        # clen (true zlib length)
    struct.pack_into("<I", out, cell.desc_off + 32, len(new_inflated))  # ulen
    delta = len(comp) - cell.clen
    if delta:
        # Patch the SAVED_ENTITIES [u32 sectionLen]; the whole tail shifts by delta and stays
        # recLen-contiguous. (chunk1_ulen is patched by the caller via len(new_chunk1).)
        sec_data_off, sec_len = find_saved_entities(chunk1)
        struct.pack_into("<I", out, sec_data_off - 4, sec_len + delta)
    return bytes(out)


# ---- component + field access inside an inflated cell ----------------------

def component_extent(cell_data, comp_name):
    """Return (dlen_off, data_off, data_len) of a component's tagged stream inside a cell.
    Component framing: [class \0][u32 0][u8 0][u32 dataLen][data]."""
    i = cell_data.find(comp_name + b"\x00")
    if i < 0:
        raise KeyError("component %s not found" % comp_name.decode())
    p = i + len(comp_name) + 1
    if struct.unpack_from("<I", cell_data, p)[0] != 0 or cell_data[p + 4] != 0:
        raise ValueError("bad component framing for %s" % comp_name.decode())
    (dlen,) = struct.unpack_from("<I", cell_data, p + 5)
    return p + 5, p + 9, dlen


def read_hero_stats(cell_data):
    """Decode the confirmed CTCHeroStats fields. Returns {name: value}."""
    _, do, dlen = component_extent(cell_data, b"CTCHeroStats")
    window = cell_data[do:do + dlen]
    out = {}
    for name, fmt in HERO_STAT_FIELDS.items():
        p = window.find(struct.pack("<I", getcrc0(name)))
        if p >= 0:
            out[name] = struct.unpack_from(fmt, window, p + 4)[0]
    return out


def set_hero_stat(cell_data, name, value):
    """Overwrite one CTCHeroStats 4-byte field in place. Returns new cell bytes (same length)."""
    if name not in HERO_STAT_FIELDS:
        raise KeyError("unknown hero stat %r (known: %s)" % (name, " ".join(HERO_STAT_FIELDS)))
    _, do, dlen = component_extent(cell_data, b"CTCHeroStats")
    p = cell_data.find(struct.pack("<I", getcrc0(name)), do, do + dlen)
    if p < 0:
        raise KeyError("stat tag for %r not found in CTCHeroStats" % name)
    fmt = HERO_STAT_FIELDS[name]
    out = bytearray(cell_data)
    struct.pack_into(fmt, out, p + 4, float(value) if fmt == "<f" else int(value))
    return bytes(out)


def _plausible_item_name(s):
    return (3 <= len(s) <= 64 and all(0x20 <= c < 0x7F for c in s)
            and (chr(s[0]).isalpha() or s[0:1] == b"_"))


def walk_inventory(cell_data, comp_name=b"CTCInventory"):
    """Walk one CTCInventory* component. Returns (item_count, records, first_rec_off) where
    records = [(name, qty, qty_off_in_cell)]. Records with ext != 0 carry a variable extension
    which is SKIPPED by scanning to the next well-formed length-prefixed name (validated:
    decoded count == declared count on all real saves)."""
    _, do, dlen = component_extent(cell_data, comp_name)
    data = cell_data[do:do + dlen]
    if len(data) < 9:
        return 0, [], do + 9
    tag, count = struct.unpack_from("<II", data, 0)
    if tag != INV_LIST_TAG:
        raise ValueError("inventory list tag mismatch: %08x" % tag)
    records = []
    o = 9                                   # [tag][count][u8 pad]
    n = len(data)
    while o + 4 <= n and len(records) < count:
        (L,) = struct.unpack_from("<I", data, o)
        if 3 <= L <= 64 and o + 4 + L + 4 <= n and _plausible_item_name(data[o + 4:o + 4 + L]):
            s = data[o + 4:o + 4 + L]
            (qty,) = struct.unpack_from("<I", data, o + 4 + L)
            if s != b"NULL" and qty <= 0xFFFF:      # "NULL" = empty weapon-augment slot
                records.append((s.decode("latin1"), qty, do + o + 4 + L))
                o += 4 + L + 4
                continue
        o += 1
    return count, records, do + 9


def set_item_qty(cell_data, def_name, qty, comp_name=b"CTCInventory"):
    """Overwrite the u32 quantity of an existing inventory item (fixed-width, in place)."""
    if not 0 <= qty <= 0xFFFF:
        raise ValueError("qty must be 0..65535 (engine values observed <= 0xFFFF)")
    _, records, _ = walk_inventory(cell_data, comp_name)
    hits = [r for r in records if r[0] == def_name]
    if not hits:
        raise KeyError("item %r not in %s" % (def_name, comp_name.decode()))
    out = bytearray(cell_data)
    struct.pack_into("<I", out, hits[0][2], qty)
    return bytes(out)


def add_item(cell_data, def_name, qty=1, comp_name=b"CTCInventory", allow_duplicate=False):
    """Insert a new inventory record (ext=0, unkA=0) at the FRONT of the item list and bump
    itemCount + the component dataLen. Returns new cell bytes (longer than input).
    Refuses to duplicate an existing stack unless allow_duplicate (use set_item_qty instead —
    engine behaviour with two records of the same def is untested)."""
    if not 1 <= qty <= 0xFFFF:
        raise ValueError("qty must be 1..65535")
    if not allow_duplicate:
        _, records, _ = walk_inventory(cell_data, comp_name)
        if any(r[0] == def_name for r in records):
            raise ValueError("%s already in %s — use set-qty (or allow_duplicate)"
                             % (def_name, comp_name.decode()))
    name_b = def_name.encode("ascii")
    if not _plausible_item_name(name_b):
        raise ValueError("implausible def name %r" % def_name)
    dlen_off, do, dlen = component_extent(cell_data, comp_name)
    tag, count = struct.unpack_from("<II", cell_data, do)
    if tag != INV_LIST_TAG:
        raise ValueError("inventory list tag mismatch: %08x" % tag)
    rec = struct.pack("<I", len(name_b)) + name_b + struct.pack("<II", qty, 0) + b"\x00"
    out = bytearray(cell_data)
    out[do + 9:do + 9] = rec                                  # insert after [tag][count][u8]
    struct.pack_into("<I", out, do + 4, count + 1)            # itemCount += 1
    struct.pack_into("<I", out, dlen_off, dlen + len(rec))    # component dataLen += len(rec)
    return bytes(out)


# ---- whole-file operations --------------------------------------------------

def hero_cell_clen_delta(raw, transform):
    """Return (orig_clen, new_clen) for the hero cell after `transform`, WITHOUT writing.
    Lets the caller check whether an edit re-deflates to the SAME compressed length (delta 0 =>
    zero downstream shift, the safest edit) before committing."""
    sf = read_save(raw)
    hc = hero_cell(find_cells(sf.chunk1))
    new_cell = transform(hc.inflated)
    return hc.clen, len(zlib.compress(new_cell, 1))


def find_gold_preserving_clen(raw, target, radius=8000):
    """Find the Money value NEAREST to `target` whose hero cell re-deflates to the ORIGINAL clen
    (zero downstream shift). Returns (value, clen) or None. The engine walks SAVED_ENTITIES
    records by recLen and advances contiguously, so an edit that keeps clen unchanged shifts
    NOTHING downstream — the safest possible rebuild. clen varies by only a few bytes across
    nearby integer gold values, so a value within a few hundred of any target almost always
    exists that hits the exact original clen."""
    sf = read_save(raw)
    hc = hero_cell(find_cells(sf.chunk1))
    orig = hc.clen
    for d in range(0, radius + 1):
        for v in ((target + d,) if d == 0 else (target + d, target - d)):
            if v < 0:
                continue
            cell = set_hero_stat(hc.inflated, "Money", v)
            if len(zlib.compress(cell, 1)) == orig:
                return v, orig
    return None


def edit_hero_cell(raw, transform):
    """Read a save, apply `transform(hero_cell_inflated) -> new_inflated`, splice the cell
    back (re-deflate + patch recLen/clen/ulen + section length; the tail SHIFTS by the size
    delta and stays recLen-contiguous — never padded, see replace_cell), rebuild + re-sign the
    container. Returns the new 307200-byte file."""
    sf = read_save(raw)
    cells = find_cells(sf.chunk1)
    hc = hero_cell(cells)
    new_cell = transform(hc.inflated)
    new_chunk1 = replace_cell(sf.chunk1, hc, new_cell)
    new_c1_ulen = len(new_chunk1)
    return build_save(sf.chunk0_ulen, new_c1_ulen, sf.chunk0 + new_chunk1)


def hero_report(raw):
    """Decode hero stats + all three inventory components from a save. Returns a dict."""
    sf = read_save(raw)
    cells = find_cells(sf.chunk1)
    hc = hero_cell(cells)
    rep = {"cell_id": hc.cell_id, "cells": len(cells), "stats": read_hero_stats(hc.inflated),
           "inventory": {}}
    for comp in INV_COMPONENTS:
        try:
            count, records, _ = walk_inventory(hc.inflated, comp)
        except KeyError:
            continue
        rep["inventory"][comp.decode()] = {
            "declared": count, "items": [(n, q) for n, q, _ in records]}
    return rep


def verify_edit(orig_raw, new_raw, expect_desc="edit"):
    """Byte-level edit audit. Asserts that between two saves:
      - both containers verify (magic, lengths, trailer sig, 300KB zero-padded slab),
      - chunk0 is byte-identical,
      - every NON-hero cell is byte-identical (compressed bytes AND inflated bytes),
      - chunk1 outside the SAVED_ENTITIES section is byte-identical,
      - inside SAVED_ENTITIES, only the hero cell record (descriptor+data) and the
        section-length dword differ.
    Returns the list of (offset, orig_bytes, new_bytes) diff ranges of the INFLATED hero cell."""
    a, b = read_save(orig_raw), read_save(new_raw)
    assert a.chunk0 == b.chunk0, "chunk0 changed"
    ca, cb = find_cells(a.chunk1), find_cells(b.chunk1)
    assert len(ca) == len(cb), "cell count changed"
    assert [c.cell_id for c in ca] == [c.cell_id for c in cb], "cell ids changed"
    ha, hb = hero_cell(ca), hero_cell(cb)
    for x, y in zip(ca, cb):
        if x is ha:
            continue
        assert a.chunk1[x.data_off:x.data_off + x.clen] == b.chunk1[y.data_off:y.data_off + y.clen], \
            "non-hero cell %x compressed bytes changed" % x.cell_id
        assert x.inflated == y.inflated, "non-hero cell %x inflated bytes changed" % x.cell_id
    # chunk1 outside SAVED_ENTITIES: identical (section length dword may differ)
    da, sa = find_saved_entities(a.chunk1)
    db, sb = find_saved_entities(b.chunk1)
    assert da == db, "SAVED_ENTITIES moved"
    assert a.chunk1[:da - 4] == b.chunk1[:db - 4], "chunk1 before SAVED_ENTITIES changed"
    assert a.chunk1[da + sa:] == b.chunk1[db + sb:], "chunk1 after SAVED_ENTITIES changed"
    # hero-cell inflated diff ranges
    diffs = []
    xa, xb = ha.inflated, hb.inflated
    if len(xa) == len(xb):
        i = 0
        while i < len(xa):
            if xa[i] != xb[i]:
                j = i
                while j < len(xa) and xa[j] != xb[j]:
                    j += 1
                diffs.append((i, xa[i:j], xb[i:j]))
                i = j
            else:
                i += 1
    else:
        # insertion edit: find common prefix/suffix
        p = 0
        while p < min(len(xa), len(xb)) and xa[p] == xb[p]:
            p += 1
        s = 0
        while s < min(len(xa), len(xb)) - p and xa[len(xa) - 1 - s] == xb[len(xb) - 1 - s]:
            s += 1
        diffs.append((p, xa[p:len(xa) - s], xb[p:len(xb) - s]))
    return diffs


# ---- optional def-name validation against the game's names.bin --------------

DEFAULT_GAME_ROOT = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters"


def load_def_names(root=None):
    """Set of all def names from <game>/data/CompiledDefs/names.bin (or None if unavailable)."""
    import os
    root = root or os.environ.get("FABLE_ROOT") or DEFAULT_GAME_ROOT
    path = os.path.join(root, "data", "CompiledDefs", "names.bin")
    if not os.path.isfile(path):
        return None
    data = open(path, "rb").read()
    (count,) = struct.unpack_from("<I", data, 8)
    names = set()
    pos = 20
    for _ in range(count):
        pos += 4
        end = data.index(b"\x00", pos)
        names.add(data[pos:end].decode("latin1"))
        pos = end + 1
    return names


# ============================================================================
# CLI
# ============================================================================

def _hexdump(b, n=48):
    return b[:n].hex()


def _print_report(rep):
    print("hero cell id=%x (of %d cells)" % (rep["cell_id"], rep["cells"]))
    print("stats:")
    for k, v in rep["stats"].items():
        print("  %-12s %s" % (k, round(v, 3) if isinstance(v, float) else v))
    for comp, inv in rep["inventory"].items():
        print("%s: declared=%d decoded=%d" % (comp, inv["declared"], len(inv["items"])))
        for n, q in inv["items"]:
            print("  %-46s x%d" % (n, q))


def _write_verified(orig_raw, new_raw, outfile, what):
    diffs = verify_edit(orig_raw, new_raw)
    open(outfile, "wb").write(new_raw)
    print("wrote %s (%d bytes) — %s" % (outfile, len(new_raw), what))
    print("byte-level audit PASS: chunk0 + all non-hero cells + non-SAVED_ENTITIES stream identical;")
    print("hero-cell inflated diff ranges (%d):" % len(diffs))
    for off, ob, nb in diffs:
        print("  @%-7d %s -> %s" % (off, ob.hex() or "(none)", nb.hex()))
    sf = read_save(new_raw)
    print("re-read OK, trailer sig=%08x VERIFIED" % sf.orig_trailer)


def main(argv):
    import os
    usage = (
        "usage:\n"
        "  save_edit.py <save>                          dump container + section summary\n"
        "  save_edit.py report   <save>                 hero stats + inventory report\n"
        "  save_edit.py set-gold <save> <out> <amount> [--exact-clen]  set hero gold (Money);\n"
        "                                               --exact-clen snaps to the nearest value\n"
        "                                               that keeps the hero cell's compressed size\n"
        "                                               (zero downstream record shift)\n"
        "  save_edit.py set-stat <save> <out> <name> <value>\n"
        "                                               name: Money Morality RenownLevel Age Fatness\n"
        "  save_edit.py set-qty  <save> <out> <DEF> <qty>   set quantity of an inventory item\n"
        "  save_edit.py add-item <save> <out> <DEF> [qty] [--force]\n"
        "                                               add item by def name (validated vs names.bin)\n"
        "NEVER writes in place: <out> must differ from <save>."
    )
    if len(argv) < 2:
        print(usage)
        return 1
    # back-compat: `save_edit.py <file>` = container dump
    if len(argv) == 2 and os.path.isfile(argv[1]):
        raw = open(argv[1], "rb").read()
        sf = read_save(raw)
        print("magic OK | +0x0C total_data_len=%d | chunk0_ulen=%d chunk1_ulen=%d | trailer sig=%08x VERIFIED"
              % (sf.sig_field, sf.chunk0_ulen, sf.chunk1_ulen, sf.orig_trailer))
        print("sections:")
        for name, data_off, sec_len in iter_sections(sf.payload):
            print("  %-16s off=%d len=%d  head=%s" % (name, data_off, sec_len,
                                                      _hexdump(sf.payload[data_off:data_off + 16], 16)))
        return 0

    cmd = argv[1]
    raw = open(argv[2], "rb").read()
    if cmd == "report":
        _print_report(hero_report(raw))
        return 0

    outfile = argv[3]
    if os.path.abspath(outfile) == os.path.abspath(argv[2]):
        print("refusing to write in place — pick a different output path")
        return 1
    if cmd == "set-gold":
        val = int(argv[4])
        exact = "--exact-clen" in argv[5:]
        oclen, nclen = hero_cell_clen_delta(raw, lambda c: set_hero_stat(c, "Money", val))
        if nclen != oclen:
            # The engine walks SAVED_ENTITIES records by recLen and advances contiguously; a
            # clen change SHIFTS every downstream record (valid framing, but a downstream shift
            # is the leading suspect for the adult->childhood revert). Offer / apply the nearest
            # Money value that preserves the original clen (zero shift).
            snap = find_gold_preserving_clen(raw, val)
            if exact and snap is not None:
                print("note: Money=%d re-deflates to clen %d (orig %d, shift %d). "
                      "--exact-clen: snapping to nearest zero-shift value Money=%d."
                      % (val, nclen, oclen, nclen - oclen, snap[0]))
                val = snap[0]
            elif snap is not None:
                print("WARNING: Money=%d changes hero cell clen %d->%d (downstream records shift "
                      "by %d). This is valid framing but a shift is the suspected childhood-revert "
                      "trigger. Nearest ZERO-SHIFT value is Money=%d — re-run with that value, or "
                      "add --exact-clen to snap automatically."
                      % (val, oclen, nclen, nclen - oclen, snap[0]))
            else:
                print("WARNING: Money=%d changes hero cell clen %d->%d (shift %d) and no nearby "
                      "clen-preserving value was found within the search radius."
                      % (val, oclen, nclen, nclen - oclen))
        new_raw = edit_hero_cell(raw, lambda c: set_hero_stat(c, "Money", val))
        _write_verified(raw, new_raw, outfile, "Money=%d" % val)
    elif cmd == "set-stat":
        name, sval = argv[4], argv[5]
        val = float(sval) if HERO_STAT_FIELDS.get(name) == "<f" else int(sval)
        new_raw = edit_hero_cell(raw, lambda c: set_hero_stat(c, name, val))
        _write_verified(raw, new_raw, outfile, "%s=%s" % (name, val))
    elif cmd == "set-qty":
        def_name, qty = argv[4], int(argv[5])
        new_raw = edit_hero_cell(raw, lambda c: set_item_qty(c, def_name, qty))
        _write_verified(raw, new_raw, outfile, "%s qty=%d" % (def_name, qty))
    elif cmd == "add-item":
        def_name = argv[4]
        rest = argv[5:]
        force = "--force" in rest
        qty = int(rest[0]) if rest and rest[0] != "--force" else 1
        names = load_def_names()
        if names is None:
            print("WARNING: names.bin not found (set FABLE_ROOT) — def name NOT validated")
        elif def_name not in names:
            if not force:
                print("ERROR: %r not in names.bin (%d defs). Use --force to override." % (def_name, len(names)))
                return 1
            print("WARNING: %r not in names.bin — forced" % def_name)
        new_raw = edit_hero_cell(raw, lambda c: add_item(c, def_name, qty, allow_duplicate=force))
        _write_verified(raw, new_raw, outfile, "add %s x%d" % (def_name, qty))
    else:
        print(usage)
        return 1
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv))
