#!/usr/bin/env python3
"""
parse_save_entities.py - Walk the FULL SAVED_ENTITIES entity graph of a Fable: The
Lost Chapters `FableSave!` file and dump every entity / component / tagged field.

Read-only. Pure stdlib (struct, zlib). Companion to docs/SAVE_ENTITY_GRAPH.md and the
container/HEADER/hero-stat docs (SAVEGAME_FORMAT.md, SAVE_HEADER_MAP.md,
SAVE_HERO_STATS.md). Reuses the seed-0 CRC field-tag hash from tools/save_tools/savecrc.py.

Nesting (all empirically validated, see docs/SAVE_ENTITY_GRAPH.md):
  file -> chunk1 (zlib) -> inflated stream containing "SAVED_ENTITIES\0"
  SAVED_ENTITIES section = [u32 sectionLen] + run of N independently zlib-compressed CELLS
  each inflated cell:
     [u32 topEntityCount]
     entity+                                 (top-level entities, laid out back to back)
  entity:
     [instance-name ASCII \0]
     [u32 A][u32 B]                          (cell/spatial fields; only on some, see notes)
     [class-name ASCII \0]                   (engine class, e.g. Object/Marker/Village/AICreature)
     [u32 flagsA][u32 four=4][u32 flagsB]
     [u64 UID]
     [u32 z]
     [DEF-name ASCII \0]                     (the game.bin def / thing type)
     [u32 baseFieldLen] baseFieldLen bytes of [tag][value] base fields
     [u32 0][u32 componentCount][u32 0]
     component * componentCount
     ... child framing / END markers ...
  component:
     [class-name ASCII \0][u32 0][u8 0][u32 dataLen] dataLen bytes of [tag][value] fields

Because instance-name / class / def are all NUL-terminated ASCII and components carry an
explicit dataLen, the walker is anchored on the ASCII class/def names and the length
fields rather than assuming fixed offsets. It records the entity inventory and, for each
component, the raw tagged-field stream so a save editor can locate a field by seed-0 CRC.
"""
import sys, os, zlib, struct, json

_HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(_HERE, 'save_tools'))
sys.path.insert(0, _HERE)
import savecrc  # getcrc(name, seed=0xFFFFFFFF); use seed 0 for field tags
import lionhead_lz  # LZO1X — QUESTS/FACTIONS persist-text sub-blobs use it (validated)

# ---------------------------------------------------------------------------
# Container -> inflated chunk1 -> SAVED_ENTITIES cells
# ---------------------------------------------------------------------------

def load_chunks(path):
    p = savecrc.parse_header(savecrc.read_file(path))
    return p['header'], p['body']


def find_saved_entities(body):
    i = body.find(b'SAVED_ENTITIES\x00')
    if i < 0:
        raise ValueError("SAVED_ENTITIES marker not found")
    name_end = i + len(b'SAVED_ENTITIES')
    sec_len = struct.unpack_from('<I', body, name_end + 1)[0]
    data_off = name_end + 5
    return data_off, sec_len


def inflate_cells(body):
    """Return list of (comp_off, comp_len, inflated_bytes) for each zlib cell in SAVED_ENTITIES."""
    data_off, sec_len = find_saved_entities(body)
    cells = []
    pos = data_off
    end = data_off + sec_len
    while pos < end:
        while pos < end and not (body[pos] == 0x78 and pos + 1 < end and body[pos + 1] in (0x01, 0x5e, 0x9c, 0xda)):
            pos += 1
        if pos >= end:
            break
        try:
            dc = zlib.decompressobj()
            out = dc.decompress(body[pos:end])
            consumed = (end - pos) - len(dc.unused_data)
            cells.append((pos, consumed, out))
            pos += consumed
        except Exception:
            pos += 1
    return cells


# ---------------------------------------------------------------------------
# Cell -> entity graph walk
# ---------------------------------------------------------------------------

def _cstr(d, o):
    e = d.index(b'\0', o)
    return d[o:e].decode('latin1'), e + 1


def _is_ascii_name(d, o, maxlen=64):
    """True if d[o:] starts with a plausible NUL-terminated ASCII identifier."""
    e = d.find(b'\0', o, o + maxlen)
    if e < 0 or e == o:
        return False
    s = d[o:e]
    return all(0x20 <= c < 0x7f for c in s)


class Component:
    def __init__(self, cls, data_off, data_len, fields):
        self.cls = cls
        self.data_off = data_off      # offset (within cell) of the tagged-field stream
        self.data_len = data_len
        self.fields = fields          # list of (tag, value_off, value_len_guess)


class Entity:
    def __init__(self):
        self.inst = None
        self.cls = None
        self.defname = None
        self.uid = None
        self.base_off = None
        self.base_len = 0
        self.components = []          # list[Component]
        self.start = None
        self.end = None


# Seed-0 tag -> known field name (for pretty printing). Confirmed elsewhere in the docs.
KNOWN_TAGS = {}
for _n in ("Money", "Morality", "Age", "Fatness", "RenownLevel", "Health",
           "Strength", "Skill", "Will", "Experience", "PlayerCharacterUID"):
    KNOWN_TAGS[savecrc.getcrc(_n, 0)] = _n


def parse_component(d, o, limit):
    """Parse one component at offset o. Returns (Component, next_off) or (None, o) if not a component.

    CONFIRMED framing (real bytes, all cells):
        [class-name ASCII \0][u32 pad=0][u8 0][u32 dataLen][dataLen bytes][u32 sep=0]
    The trailing u32 separator (=0) sits between adjacent components; next_off skips it."""
    if not _is_ascii_name(d, o):
        return None, o
    cls, p = _cstr(d, o)
    if not cls.startswith('CTC') and not cls.startswith('C'):
        # component class names in this engine are CTC*/C* ; bail otherwise
        return None, o
    # framing after class name: [u32 0][u8 0][u32 dataLen]
    if p + 9 > limit:
        return None, o
    pad = struct.unpack_from('<I', d, p)[0]
    b5 = d[p + 4]
    dlen = struct.unpack_from('<I', d, p + 5)[0]
    data_off = p + 9
    if pad != 0 or b5 != 0 or data_off + dlen + 4 > limit:
        return None, o
    fields = scan_fields(d, data_off, data_off + dlen)
    # +4 = the inter-component u32 separator (always 0 on real bytes)
    return Component(cls, data_off, dlen, fields), data_off + dlen + 4


def scan_fields(d, o, end):
    """Heuristically split a tagged-field stream into [tag][value] records.
    Values are not self-describing, so we record tag offsets only (value length left to
    the type-aware editor). We stop at 'END'/non-tag noise. Returns list of (tag, val_off)."""
    out = []
    i = o
    while i + 4 <= end:
        tag = struct.unpack_from('<I', d, i)[0]
        out.append((tag, i + 4))
        # We cannot know value width generically; advance by 4 as a coarse cursor.
        i += 4
    return out


# ---- Census helpers (robust, name-anchored) -------------------------------

def iter_component_offsets(d):
    """Yield (offset, class_name, data_off, data_len) for every well-framed component in a cell."""
    n = len(d)
    i = 0
    while True:
        j = d.find(b'CTC', i)
        if j < 0:
            # also C-prefixed non-CTC components are rare; skip for census clarity
            break
        comp, nxt = parse_component(d, j, n)
        if comp is not None:
            yield j, comp.cls, comp.data_off, comp.data_len
            i = nxt
        else:
            i = j + 3


def iter_defnames(d):
    """Yield (offset, defname) for tokens that look like DEF-names: UPPER_SNAKE ASCII \0
    preceded by the entity framing. Heuristic census aid."""
    import re
    n = len(d)
    for m in re.finditer(rb'[A-Za-z][A-Za-z0-9_]{2,63}\x00', d):
        s = m.group()[:-1]
        yield m.start(), s.decode('latin1')


# ---------------------------------------------------------------------------
# High-level: full graph parse for a save
# ---------------------------------------------------------------------------

def parse_save(path):
    hdr, body = load_chunks(path)
    cells = inflate_cells(body)
    return hdr, body, cells


def census(cells):
    """Build the entity-type census across all cells:
       - class-name counts (engine classes: Object/Marker/Village/AICreature/Building/...)
       - component class counts (CTC*)
       - def-name samples."""
    from collections import Counter
    comp_counter = Counter()
    for _, _, d in cells:
        for off, cls, do, dl in iter_component_offsets(d):
            comp_counter[cls] += 1
    return comp_counter


# The distinct engine "class" tokens seen at entity headers. Note AICreature CONTAINS
# 'Creature' as a substring, so we must anchor on a preceding NUL (word boundary) and demand
# the strict header shape to avoid mis-anchoring on nested child records / component names.
def def_census(cells):
    """Count DEF-names (thing types) across ALL entity records (incl. children/hero)."""
    from collections import Counter
    c = Counter()
    for ci, defname, uid, off, bl in entity_records(cells):
        c[defname] += 1
    return c


# DEF-name prefixes that are real game.bin thing defs (validated: record count with
# this whitelist == the cell's END\0 entity-terminator count, byte-exact, both saves).
DEF_PREFIXES = ('OBJECT_', 'CREATURE_', 'MARKER_', 'BUILDING_', 'VILLAGE_',
                'DUMMY_', 'THING_', 'HOLDING_')


def entity_records(cells):
    """Class-independent full entity enumeration (top-level AND children AND the hero,
    whose header carries no engine-class token). Anchor, validated on real bytes:
        [u64 UID (hi dword nonzero)][u32 0][DEF-name UPPER_SNAKE\\0][u32 baseFieldLen]
    with DEF-name starting with a known def prefix. Yields (cell, defname, uid, def_off,
    baseFieldLen). Cross-check: len == total END\\0 markers (348 in Cornelio/Manual-Save1)."""
    import re
    defre = re.compile(rb'(?<=\x00)[A-Z][A-Z0-9_]{2,79}\x00')
    out = []
    for ci, (_, _, d) in enumerate(cells):
        for m in defre.finditer(d):
            o = m.start()
            if o < 12 or m.end() + 4 > len(d):
                continue
            nm = m.group()[:-1].decode('latin1')
            if not nm.startswith(DEF_PREFIXES):
                continue
            if struct.unpack_from('<I', d, o - 4)[0] != 0:
                continue
            uid = struct.unpack_from('<Q', d, o - 12)[0]
            if uid == 0 or (uid >> 32) == 0:
                continue
            bl = struct.unpack_from('<I', d, m.end())[0]
            if bl > 0x4000:
                continue
            out.append((ci, nm, uid, o, bl))
    return out


# ---------------------------------------------------------------------------
# Hero inventory (CTCInventory / CTCInventoryClothing / CTCInventoryWeapons ...)
# ---------------------------------------------------------------------------
# CONFIRMED on-disk layout of a CTCInventory* component's tagged-field data block:
#   [u32 tag = getcrc("Items",0)? -> observed 2b837863][u32 itemCount]
#   itemCount x item-record.
# Item record (name+qty CONFIRMED; tail variable / [hypothesis]):
#   [u8 lead][u32 nameLen][nameLen ASCII chars (no NUL)][u32 quantity][variable tail]
# The tail is 4 bytes for most items but longer for some (e.g. money bag), so we parse
# item name + quantity by a length-prefix scan (robust) and stop at itemCount.
INV_LIST_TAG = 0x2b837863  # = getcrc("InventoryItems", 0) — CONFIRMED name match


def component_data(d, comp_name):
    """Return (data_off, data_len, data_bytes) of a component's tagged stream in cell d."""
    i = d.find(comp_name + b'\x00')
    if i < 0:
        return None
    p = i + len(comp_name) + 1
    dlen = struct.unpack_from('<I', d, p + 5)[0]
    do = p + 9
    return do, dlen, d[do:do + dlen]


def _plausible_item_name(s):
    return (len(s) >= 3 and all(0x20 <= c < 0x7f for c in s)
            and (s[:1].isalpha() or s[:1] == b'_'))


def decode_inventory(d, comp_name=b'CTCInventory'):
    """Decode one CTCInventory* component. Returns (count, items, extras) where
    items = [(name, qty)], extras = dict with 'names_after' (the NUL-terminated
    name table that follows the item records — quick-access/equipped assignments).

    CONFIRMED list framing (real bytes, both Cornelio saves + Test):
        [u32 tag = getcrc("InventoryItems",0) = 2b837863][u32 itemCount][u8]
        itemCount x record
    CONFIRMED record core:  [u32 nameLen][name chars, no NUL][u32 quantity]
    then [u32 unkA][u8 ext]; ext!=0 => variable extension payload (per-item-class
    extra state: money-bag contents, weapon augmentation sublists [u32 n][records],
    hotkey CRCs...). The extension is skipped by scanning to the next well-formed
    length-prefixed name (robust — validated: decodes count==parsed on all saves).
    After the last record: run of NUL-terminated item names (see extras), then
    additional [tag][value] fields of the component (e.g. the equipped-weapon
    name cstr in CTCInventoryWeapons)."""
    sec = component_data(d, comp_name)
    if sec is None:
        return 0, [], {}
    _, dlen, data = sec
    if len(data) < 9:
        return 0, [], {}
    tag, count = struct.unpack_from('<II', data, 0)
    items = []
    o = 8
    n = len(data)
    last_end = 8
    while o + 4 <= n:
        L = struct.unpack_from('<I', data, o)[0]
        if 3 <= L <= 64 and o + 4 + L + 4 <= n and _plausible_item_name(data[o + 4:o + 4 + L]):
            s = data[o + 4:o + 4 + L]
            qo = o + 4 + L
            qty = struct.unpack_from('<I', data, qo)[0]
            if s != b'NULL' and qty <= 0xFFFF:   # "NULL" = empty augment slot
                items.append((s.decode('latin1'), qty))
                o = qo + 4
                last_end = o
                continue
        o += 1
    # trailing NUL-terminated name table (quick-access / equipped assignments)
    names_after = []
    j = last_end
    while j < n:
        k = data.find(b'\x00', j)
        if k < 0:
            break
        seg = data[j:k]
        st = seg.rfind(b'OBJECT_')
        if st >= 0 and _plausible_item_name(seg[st:]):
            names_after.append(seg[st:].decode('latin1'))
        j = k + 1
    return count, items, {'names_after': names_after, 'dataLen': dlen}


# ---------------------------------------------------------------------------
# LZ-text sub-blobs (QUESTS / FACTIONS) — Lionhead LZO1X (tools/lionhead_lz.py)
# ---------------------------------------------------------------------------
# CONFIRMED framing of every LZ-text blob in the save body (30 instances parsed
# byte-exact: 1 QUESTS + 29 FACTIONS): [u32 ulen][u16 clen][clen bytes LZO1X
# stream][3 raw literal bytes]. The LZO stream self-terminates 3 bytes short of
# ulen; the last 3 bytes of the text are stored raw right after the stream.

def lz_text_blob(d, o):
    """Decode one [u32 ulen][u16 clen][LZO stream][raw tail] blob at offset o.
    Returns (text_bytes, next_off)."""
    ulen = struct.unpack_from('<I', d, o)[0]
    clen = struct.unpack_from('<H', d, o + 4)[0]
    out = lionhead_lz.decompress(d[o + 6:o + 6 + clen], None)
    tail = ulen - len(out)
    if tail < 0 or tail > 16:
        raise ValueError("lz_text_blob: bad tail %d at %d" % (tail, o))
    txt = out + d[o + 6 + clen:o + 6 + clen + tail]
    return txt, o + 6 + clen + tail


def parse_quests(body):
    """QUESTS section = [lz-text blob (the whole quest persist-script)] +
    binary tagged trailer. Returns (text, trailer_bytes)."""
    sec = find_section(body, b'QUESTS')
    if not sec:
        return None, None
    doff, seclen, d = sec
    txt, nxt = lz_text_blob(d, 0)
    return txt, d[nxt:seclen]


def parse_quest_trailer(tr):
    """Best-effort typed split of the QUESTS binary trailer:
    [u32 tag] + value, where value is a NUL-terminated quest name when the next
    bytes are ASCII 'Q_'/'C'/... else u32 (heuristic) else u8. Field NAMES for
    these tags are still unrecovered (2 wordlist brutes failed) — structure only."""
    out = []
    o = 0
    n = len(tr)
    while o + 4 <= n:
        tag = struct.unpack_from('<I', tr, o)[0]
        o += 4
        # cstr?
        e = tr.find(b'\x00', o)
        if e > o and e - o >= 3 and all(0x20 <= c < 0x7f for c in tr[o:e]):
            out.append((tag, 'cstr', tr[o:e].decode('latin1')))
            o = e + 1
            continue
        if o + 4 <= n:
            v = struct.unpack_from('<I', tr, o)[0]
            if v <= 0xFFFF:                      # small int => u32 field
                out.append((tag, 'u32', v))
                o += 4
                continue
        out.append((tag, 'u8', tr[o] if o < n else None))
        o += 1
    return out


def parse_factions(body):
    """FACTIONS section: [u32 headTag=4f0e45c3][u32 count] then count x
    [u32 recTag][lz-text blob]. Text is 'Name "FACTION_X";\\nNumberOfAllies n;
    Ally "...";...EndFaction'. Validated: 29/29 records consume the section
    byte-exact. recTag names unrecovered (not getcrc(factionName,0))."""
    sec = find_section(body, b'FACTIONS')
    if not sec:
        return None, []
    doff, seclen, d = sec
    head_tag, count = struct.unpack_from('<II', d, 0)
    recs = []
    o = 8
    for _ in range(count):
        tag = struct.unpack_from('<I', d, o)[0]
        txt, o = lz_text_blob(d, o + 4)
        recs.append((tag, txt))
    return head_tag, recs


def lzo_store(D):
    """Emit a VALID LZO1X stream that decodes to D using (almost) only literal runs —
    the write path for QUESTS/FACTIONS text without a real compressor. Structure:
      [initial literal run of k][one 3-byte M3 match, dist d, tl=0][literal run of rest][EOS]
    where D[k:k+3] must equal D[k-d:k-d+3] (a repeated 3-gram; always present in the
    persist-text). Validated: lionhead_lz.decompress(lzo_store(D)) == D for the real
    QUESTS text and all 29 FACTIONS texts."""
    n = len(D)
    if n <= 238:
        return bytes([0x11 + n]) + D + b'\x11\x00\x00'

    def lit_run_token(m):
        # loop-top literal-run token for m literals (m==0 -> nothing; m>=4 required;
        # m in 4..18 -> single token m-3; m>=19 -> 0-token + extension)
        if m == 0:
            return b''
        if 4 <= m <= 18:
            return bytes([m - 3])
        if m >= 19:
            rem = m - 18
            z, last = divmod(rem, 255)
            if last == 0:
                z, last = z - 1, 255
            return b'\x00' + b'\x00' * z + bytes([last])
        raise ValueError("cannot encode literal run of %d" % m)

    for k in range(4, 236):
        if not (k + 3 <= n and (n - k - 3 == 0 or n - k - 3 >= 4)):
            continue
        tri = D[k:k + 3]
        j = D.rfind(tri, 0, k)          # need d = k-j in 1..0x3fff
        if j < 0 or k - j > 0x3fff:
            continue
        d = k - j
        e = d - 1                        # M3 encoded distance
        lo = (e & 0x3f) << 2             # low 2 bits 0 => 0 trailing literals
        hi = e >> 6
        m = n - k - 3
        return (bytes([0x11 + k]) + D[:k] + bytes([0x21, lo, hi]) +
                lit_run_token(m) + D[k + 3:] + b'\x11\x00\x00')
    raise ValueError("lzo_store: no repeated 3-gram in first 238 bytes")


def build_lz_text_blob(T):
    """Inverse of lz_text_blob: [u32 ulen][u16 clen][stream for T[:-3]][last 3 bytes raw]."""
    stream = lzo_store(T[:-3])
    if len(stream) > 0xFFFF:
        raise ValueError("compressed stream too long for u16 clen")
    return struct.pack('<IH', len(T), len(stream)) + stream + T[-3:]


def replace_quests_text(body, new_text):
    """Return a new inflated chunk1 body with the QUESTS persist-text replaced.
    Preserves the binary trailer; patches the QUESTS [u32 sectionByteLen]."""
    m = body.find(b'QUESTS\x00')
    seclen = struct.unpack_from('<I', body, m + 7)[0]
    doff = m + 11
    d = body[doff:doff + seclen]
    _, nxt = lz_text_blob(d, 0)
    trailer = d[nxt:]
    blob = build_lz_text_blob(new_text)
    new_sec = blob + trailer
    return (body[:m + 7] + struct.pack('<I', len(new_sec)) +
            new_sec + body[doff + seclen:])


def parse_regions(body):
    """REGIONS section: [u32 a][u32 b][u32 c] then run of
    [region-name ASCII\\0][19 bytes flags] records. Returns (head, [(name, flagbytes)])."""
    sec = find_section(body, b'REGIONS')
    if not sec:
        return None, []
    doff, seclen, d = sec
    head = struct.unpack_from('<III', d, 0)
    recs = []
    o = 12
    n = seclen
    while o < n:
        e = d.find(b'\x00', o)
        if e < 0 or e == o:
            break
        nm = d[o:e]
        if not all(0x20 <= c < 0x7f for c in nm):
            break
        recs.append((nm.decode('latin1'), d[e + 1:e + 1 + 19]))
        o = e + 1 + 19
    return head, recs


# ---------------------------------------------------------------------------
# Body sections (PLAYER binary + QUESTS/FACTIONS LZ-text + REGIONS binary)
# ---------------------------------------------------------------------------

def find_section(body, name):
    """Return (data_offset, section_len, data_bytes) for a top-level body section, or None.
    Section framing: [name ASCII \0][u32 sectionByteLen][data]."""
    m = body.find(name + b'\x00')
    if m < 0:
        return None
    off = m + len(name) + 1
    seclen = struct.unpack_from('<I', body, off)[0]
    doff = off + 4
    return doff, seclen, body[doff:doff + seclen]


def classify_section(data):
    """Heuristic: 'binary' (tagged / clean strings) vs 'lz-text' (game's LZ-compressed
    persist-text: readable fragments interleaved with copy tokens)."""
    if len(data) < 12:
        return 'binary'
    # LZ-text sections start [u32 ulen][u32 clen] then \r\n and carry partial words with
    # sub-0x20 copy-token bytes sprinkled through otherwise-printable text.
    head = data[8:12]
    if head[:2] == b'\r\n' or head[:1] == b'\n':
        return 'lz-text'
    return 'binary'


def quest_fragments(body):
    """Best-effort quest state from the QUESTS section. QUESTS is LZ-compressed persist-text
    (see docs) so only ASCII fragments survive without the codec. Returns readable tokens."""
    import re
    sec = find_section(body, b'QUESTS')
    if not sec:
        return []
    _, _, d = sec
    toks = set()
    for pat in (rb'CS_[A-Za-z0-9_]+', rb'Q_[A-Za-z0-9_]+', rb'START_QUEST_CARDS'):
        for m in re.findall(pat, d):
            toks.add(m.decode('latin1'))
    return sorted(toks)


ENGINE_CLASSES = (b'Object', b'Marker', b'Village', b'AICreature', b'Building',
                  b'Creature', b'Region', b'Holding')

import re as _re
_DEF_RE = _re.compile(rb'[A-Z][A-Z0-9_]{2,79}\x00')


def _looks_like_def(d, o):
    """DEF-names are UPPER_SNAKE (OBJECT_*, CREATURE_*, MARKER_*, VILLAGE_*, BUILDING_*)."""
    m = _DEF_RE.match(d, o)
    return m is not None


def entity_headers(cells):
    """Extract top-level entity headers per cell: (cell_idx, inst, class, defname, uid).

    CONFIRMED header shape (real bytes):
        [inst-name ASCII \0][u32 A][u32 B]<EngineClass>\0[u32 flagsA][u32 mid][u32 flagsB]
        [u64 UID][u32 z=0][DEF-name UPPER_SNAKE \0][u32 baseFieldLen]
    `mid` is 4 (fresh) or 0xffffffff (no-parent marker); `z` is 0. Anchor demands a
    NUL before the class token and a UPPER_SNAKE DEF after the 24-byte middle block, which
    rejects the substring/nested false positives the naive scan produced."""
    rows = []
    for ci, (_, _, d) in enumerate(cells):
        n = len(d)
        for cls in ENGINE_CLASSES:
            start = 0
            while True:
                p = d.find(cls + b'\x00', start)
                if p < 0:
                    break
                start = p + 1
                # class token must be at a word boundary (preceded by NUL) to avoid the
                # 'Creature' inside 'AICreature' and matches inside component data.
                if p == 0 or d[p - 1] != 0:
                    continue
                after = p + len(cls) + 1
                if after + 24 > n:
                    continue
                mid = struct.unpack_from('<I', d, after + 4)[0]
                z = struct.unpack_from('<I', d, after + 20)[0]
                if mid not in (4, 0xffffffff) or z != 0:
                    continue
                uid = struct.unpack_from('<Q', d, after + 12)[0]
                def_off = after + 24
                if not _looks_like_def(d, def_off):
                    continue
                defname, _ = _cstr(d, def_off)
                # instance name precedes the class token: walk back to prior \0
                pn = d.rfind(b'\x00', max(0, p - 80), p)
                inst = d[pn + 1:p].decode('latin1') if pn >= 0 else '?'
                inst = ''.join(ch for ch in inst if 0x20 <= ord(ch) < 0x7f)
                rows.append((ci, inst, cls.decode(), defname, uid))
    return rows


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    if len(sys.argv) < 2:
        print("usage: parse_save_entities.py <save> [command]")
        print("commands: cells | census | defcensus | entities | comps <cellIdx> |")
        print("          dump <cellIdx> | herostats | inventory | sections |")
        print("          quests | factions [full] | regions | report |")
        print("          flipquest <questName> <outfile>   (write-path PoC)")
        return
    path = sys.argv[1]
    cmd = sys.argv[2] if len(sys.argv) > 2 else 'census'
    hdr, body, cells = parse_save(path)

    if cmd == 'cells':
        print("SAVED_ENTITIES: %d cells, total inflated %d bytes" %
              (len(cells), sum(len(c[2]) for c in cells)))
        for i, (co, cl, d) in enumerate(cells):
            tc = struct.unpack_from('<I', d, 0)[0]
            print("  cell %2d comp_off=%d comp_len=%d inflated=%d topEntities=%d" %
                  (i, co, cl, len(d), tc))

    elif cmd == 'census':
        cc = census(cells)
        print("Component-class census (CTC*) across %d cells:" % len(cells))
        for cls, n in cc.most_common():
            print("  %-40s %d" % (cls, n))
        print("total distinct component classes:", len(cc))

    elif cmd == 'entities':
        rows = entity_headers(cells)
        print("Top-level entity headers (cell, inst, class, def, uid):")
        for ci, inst, cls, defname, uid in rows:
            print("  cell%2d %-10s %-10s %-38s uid=%016x" % (ci, cls, inst[:10], defname[:38], uid))
        print("total:", len(rows))

    elif cmd == 'comps':
        ci = int(sys.argv[3])
        d = cells[ci][2]
        print("cell %d components:" % ci)
        for off, cls, do, dl in iter_component_offsets(d):
            tags = [struct.unpack_from('<I', d, do + k)[0] for k in range(0, min(dl, 32), 4)]
            print("  @%6d %-32s dataLen=%-5d first_tags=%s" %
                  (off, cls, dl, ' '.join('%08x' % t for t in tags[:4])))

    elif cmd == 'dump':
        ci = int(sys.argv[3])
        d = cells[ci][2]
        sys.stdout.buffer.write(d)

    elif cmd == 'defcensus':
        dc = def_census(cells)
        ends = sum(c[2].count(b'END\x00') for c in cells)
        print("DEF-name (thing-type) census across ALL entity records (incl. children):")
        for nm, n in dc.most_common():
            print("  %-46s %d" % (nm, n))
        print("total records: %d (END-marker cross-check: %d), distinct def-names: %d" %
              (sum(dc.values()), ends, len(dc)))

    elif cmd == 'flipquest':
        # PoC write path: insert a quest name into the COMPLETED_QUESTS text block,
        # rebuild the container (re-deflate + re-sign) and re-verify. Usage:
        #   flipquest <questName> <outfile>
        import save_edit
        qname = sys.argv[3]
        outfile = sys.argv[4]
        raw = open(path, 'rb').read()
        sf = save_edit.read_save(raw)
        txt, trailer = parse_quests(body)
        marker = b'START_COMPLETED_QUESTS'
        i = txt.index(marker) + len(marker)
        new_txt = txt[:i] + b'\r\n' + qname.encode() + txt[i:]
        new_body = replace_quests_text(body, new_txt)
        payload = sf.chunk0 + new_body
        out = save_edit.build_save(sf.chunk0_ulen, len(new_body), payload)
        open(outfile, 'wb').write(out)
        # verify: re-read + re-decode
        sf2 = save_edit.read_save(out)
        _, body2, _ = parse_save(outfile)
        txt2, tr2 = parse_quests(body2)
        assert qname.encode() in txt2 and tr2 == trailer, "verify failed"
        print("wrote %s (%d bytes), trailer sig re-verified, quest %r now in COMPLETED_QUESTS"
              % (outfile, len(out), qname))

    elif cmd == 'inventory':
        heroidx = next(i for i, c in enumerate(cells) if b'CTCInventory\x00' in c[2])
        d = cells[heroidx][2]
        print("hero cell idx", heroidx)
        for comp in (b'CTCInventory', b'CTCInventoryClothing', b'CTCInventoryWeapons'):
            count, items, extras = decode_inventory(d, comp)
            print("\n%s: count=%d, decoded=%d (dataLen=%s)" %
                  (comp.decode(), count, len(items), extras.get('dataLen')))
            for nm, qty in items:
                print("  %-44s x%d" % (nm, qty))
            if extras.get('names_after'):
                print("  -- trailing name table (quick-access/equipped refs):")
                for nm in extras['names_after']:
                    print("     %s" % nm)

    elif cmd == 'sections':
        print("Top-level body sections (name, len, class):")
        pos = 0
        for name in (b'PLAYER', b'QUESTS', b'REGIONS', b'FACTIONS'):
            sec = find_section(body, name)
            if not sec:
                print("  %-10s ABSENT" % name.decode()); continue
            doff, seclen, data = sec
            print("  %-10s @%-7d len=%-6d %s" %
                  (name.decode(), doff, seclen, classify_section(data)))

    elif cmd == 'quests':
        txt, trailer = parse_quests(body)
        sys.stdout.write(txt.decode('latin1'))
        print("\n---- binary trailer (%d bytes) ----" % len(trailer))
        for tag, kind, v in parse_quest_trailer(trailer):
            print("  tag=%08x %-5s %s" % (tag, kind, v))

    elif cmd == 'factions':
        head_tag, recs = parse_factions(body)
        print("FACTIONS head tag=%08x, %d records" % (head_tag, len(recs)))
        full = len(sys.argv) > 3 and sys.argv[3] == 'full'
        import re
        for tag, txt in recs:
            if full:
                print("---- tag=%08x ----" % tag)
                sys.stdout.write(txt.decode('latin1'))
            else:
                m = re.search(rb'Name "([^"]+)"', txt)
                print("  tag=%08x %-36s (%d bytes text)" %
                      (tag, m.group(1).decode() if m else '?', len(txt)))

    elif cmd == 'regions':
        head, recs = parse_regions(body)
        print("REGIONS head=%s, %d records:" % (head, len(recs)))
        for nm, fl in recs:
            print("  %-28s %s" % (nm, fl.hex()))

    elif cmd == 'report':
        # one-shot dump of everything (used to build docs/SAVE_ENTITY_GRAPH.md)
        print("== cells =="); print("%d cells, %d bytes inflated" %
                                    (len(cells), sum(len(c[2]) for c in cells)))
        rows = entity_headers(cells)
        from collections import Counter
        print("== entity classes (classed top-levels) ==")
        print(Counter(c for _, _, c, _, _ in rows))
        recs = entity_records(cells)
        ends = sum(c[2].count(b'END\x00') for c in cells)
        print("== entity records: %d (classed headers %d, END cross-check %d) ==" %
              (len(recs), len(rows), ends))
        print("   distinct DEF-names: %d" % len(set(r[1] for r in recs)))
        print("== component census (top 20) ==")
        for cls, n in census(cells).most_common(20):
            print("  %-40s %d" % (cls, n))
        heroidx = next(i for i, c in enumerate(cells) if b'CTCHeroStats\x00' in c[2])
        print("== hero cell: %d ==" % heroidx)
        for comp in (b'CTCInventory', b'CTCInventoryClothing', b'CTCInventoryWeapons'):
            count, items, extras = decode_inventory(cells[heroidx][2], comp)
            print("  %-24s count=%d decoded=%d" % (comp.decode(), count, len(items)))
        txt, trailer = parse_quests(body)
        print("== quests: %d bytes text, %d bytes trailer ==" % (len(txt), len(trailer)))
        ht, recs = parse_factions(body)
        print("== factions: %d ==" % len(recs))
        _, rg = parse_regions(body)
        print("== regions: %d ==" % len(rg))

    elif cmd == 'herostats':
        # locate hero cell and decode the confirmed CTCHeroStats fields
        heroidx = next(i for i, c in enumerate(cells) if b'CTCHeroStats\x00' in c[2])
        d = cells[heroidx][2]
        print("hero cell idx", heroidx)
        for name, kind in (("Money", "i32"), ("Morality", "i32"), ("Age", "f32"),
                           ("Fatness", "f32"), ("RenownLevel", "i32")):
            tag = savecrc.getcrc(name, 0)
            tle = struct.pack('<I', tag)
            # search only within the CTCHeroStats component window
            hs = d.find(b'CTCHeroStats\x00')
            p = d.find(tle, hs)
            if p < 0:
                print("  %-12s tag=%08x NOT FOUND" % (name, tag)); continue
            vo = p + 4
            if kind == 'i32':
                v = struct.unpack_from('<i', d, vo)[0]
            else:
                v = round(struct.unpack_from('<f', d, vo)[0], 3)
            print("  %-12s tag=%08x @%d = %s" % (name, tag, p, v))


if __name__ == '__main__':
    main()
