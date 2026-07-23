"""Survey + validate the hero appearance/clothing system in game.bin.

Decodes every OBJECT's CDef listing, extracts each CAppearanceModifierDef
(HideMaterials / Graphics / CoversBodyAreaFlags / HeroSuit / SuitPart /
AppearanceType / ArmourDefIndex), resolves the Graphics model IDs against
graphics.big MBANK_ALLMESHES, and prints the full item -> modifier -> mesh
chain. Evidence backing docs/APPEARANCE_SYSTEM.md.

Usage:  python tools/appearance_survey.py [game-root]
"""
import struct
import sys
import zlib
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_bigb

DEFAULT_ROOT = r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters'


# --- game.bin / names.bin readers (mirror of FableForge forge::bin) ---------

def load_names(path):
    data = open(path, 'rb').read()
    count = struct.unpack_from('<I', data, 8)[0]
    byoff = {}
    pos = 20
    for _ in range(count):
        off = pos - 20 + 4
        pos += 4
        start = pos
        while data[pos] != 0:
            pos += 1
        byoff[off] = data[start:pos].decode('latin1')
        pos += 1
    return byoff


def load_bin(root, bin_name='game.bin'):
    names = load_names(os.path.join(root, 'data', 'CompiledDefs', 'names.bin'))
    b = open(os.path.join(root, 'data', 'CompiledDefs', bin_name), 'rb').read()
    entry_count = struct.unpack_from('<I', b, 9)[0]
    main = 13
    ccpos = main + entry_count * 12
    chunk_count = struct.unpack_from('<I', b, ccpos)[0]
    ctab = ccpos + 4
    data_off = ctab + chunk_count * 8 + 4
    entries = []
    for c in range(1, chunk_count):
        fe, cs = struct.unpack_from('<ii', b, ctab + (c - 1) * 8)
        ee, ce = struct.unpack_from('<ii', b, ctab + c * 8)
        if ce - cs <= 0 or ee - fe <= 0:
            continue
        infl = zlib.decompress(b[data_off + cs:data_off + ce])
        for local in range(ee - fe):
            ti = fe + local
            defref, nameref, _idx = struct.unpack_from('<iii', b, main + ti * 12)
            start = struct.unpack_from('<H', infl, local * 2)[0]
            end = len(infl)
            if local + 1 < ee - fe:
                end = struct.unpack_from('<H', infl, (local + 1) * 2)[0]
            entries.append(dict(index=ti,
                                definition=names.get(defref, ''),
                                name=names.get(nameref, '') if nameref != -1 else '',
                                data=infl[start:end]))
    return names, entries


def crc0(s):
    """Fable def-tag hash: CRC-32 (poly EDB88320) with seed 0, no final xor.
    Used for BOTH field tags and the CDef-listing class-name keys."""
    crc = 0
    for ch in s.encode():
        crc ^= ch
        for _ in range(8):
            crc = (crc >> 1) ^ (0xEDB88320 if crc & 1 else 0)
    return crc


def parse_cdef_listing(data):
    """Top-type (OBJECT/THING/CREATURE/...) payload prefix:
    01 00 01 + u16 count + count * {u32 crc0(defClass), i32 dataEntry, i32 ownerEntry}.
    dataEntry/ownerEntry are game.bin entry indices."""
    if len(data) < 5:
        return None
    count = struct.unpack_from('<H', data, 3)[0]
    if count == 0 or count > 64 or 5 + count * 12 > len(data):
        return None
    return [struct.unpack_from('<IiI', data, 5 + i * 12) for i in range(count)]


# --- CAppearanceModifierDef field extraction (tag-scan) ---------------------

def _find_tag(d, name, start=0):
    i = d.find(struct.pack('<I', crc0(name)), start)
    return -1 if i < 0 else i + 4


def parse_appearance_modifier(d):
    out = {}
    p = _find_tag(d, 'HideMaterials')
    if p >= 0:
        n = struct.unpack_from('<I', d, p)[0]; p += 4
        mats = []
        for _ in range(n):
            e0 = d.index(b'\x00', p)
            mats.append(d[p:e0].decode('latin1'))
            p = e0 + 1
        out['HideMaterials'] = mats
    p = _find_tag(d, 'Graphics')
    if p >= 0:
        n = struct.unpack_from('<I', d, p)[0]; p += 4
        recs = []
        for _ in range(n):
            # CAppearanceModifierGraphics::CEntry, 24 bytes:
            # {i32 morphStatA, i32 morphStatB, i32 modelId, f32 scale,
            #  f32 appear, f32 disappear}  (ranges vs normalized hero stat)
            recs.append(struct.unpack_from('<iiifff', d, p)); p += 24
        out['Graphics'] = recs
    for f in ('CoversBodyAreaFlags', 'HeroSuit', 'SuitPart', 'AppearanceType',
              'ArmourDefIndex'):
        p = _find_tag(d, f)
        out[f] = struct.unpack_from('<i', d, p)[0] if p >= 0 else None
    p = _find_tag(d, 'IsRemoveable')
    out['IsRemoveable'] = d[p] if p >= 0 else None
    return out


def load_mesh_ids(root):
    b = open(os.path.join(root, 'data', 'graphics', 'graphics.big'), 'rb').read()
    _, _, foot_off, _ = parse_bigb.parse_header(b)
    subs, _ = parse_bigb.parse_footer(b, foot_off)
    out = {}
    for sub in subs:
        if sub['name'] != 'MBANK_ALLMESHES':
            continue
        es, *_ = parse_bigb.parse_toc(b, sub, foot_off)
        for e in es:
            out[e['id']] = (e['name'], e['type'])
    return out


def main():
    root = sys.argv[1] if len(sys.argv) > 1 else DEFAULT_ROOT
    names, entries = load_bin(root)
    byidx = {e['index']: e for e in entries}
    mesh_by_id = load_mesh_ids(root)
    amd_crc = crc0('CAppearanceModifierDef')
    inv_crc = crc0('CInventoryItemDef')

    total = ok = 0
    for e in entries:
        if e['definition'] != 'OBJECT' or not e['name']:
            continue
        recs = parse_cdef_listing(e['data'])
        if not recs:
            continue
        amd = inv = None
        for crc, de, owner in recs:
            if crc == amd_crc:
                amd = de
            elif crc == inv_crc:
                inv = de
        if amd is None:
            continue
        total += 1
        a = parse_appearance_modifier(byidx[amd]['data'])
        suit = a['HeroSuit']
        suitname = byidx[suit]['name'] if suit and suit in byidx else str(suit)
        gfx = []
        good = True
        for sa, sb, mid, scale, ap, dis in a.get('Graphics', []):
            r = mesh_by_id.get(mid)
            good &= r is not None
            gfx.append(f'{mid}:{r[0] if r else "<MISSING>"}'
                       f'(t{r[1] if r else "?"}) a={sa} b={sb} s={scale:g} '
                       f'ap={ap:g} dis={dis:g}')
        ok += good
        print(f'{e["index"]:5d} {e["name"]:44s} amd={amd} inv={inv} '
              f'suit={suitname} part={a["SuitPart"]} type={a["AppearanceType"]} '
              f'covers={a["CoversBodyAreaFlags"]:#x} armour={a["ArmourDefIndex"]} '
              f'hide={a.get("HideMaterials", [])}')
        for g in gfx:
            print(f'          -> {g}')
    print(f'\n{total} OBJECTs carry a CAppearanceModifierDef; '
          f'{ok} resolve every Graphics model ID in MBANK_ALLMESHES.')


if __name__ == '__main__':
    main()
