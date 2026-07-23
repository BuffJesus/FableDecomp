#!/usr/bin/env python3
r"""Hero appearance OVERLAY authoring -- custom tattoos / war-paint / skin
variants through the CHeroMorphDef.TextureMorphs compositing system.

WHAT IT DOES (docs/HERO_MORPH.md is the map):
  Given a transparent PNG glyph + a body region + one or more trigger stats it
  stages, WITHOUT touching the install:
    (a) the glyph as a NEW textures.big entry in GBANK_MAIN_PC
        (DXT3, TransparencyType=1 -- exactly the retail COMP_HERO_* overlay
        format; alpha channel = compositing mask, RGB = overlay colour), and
    (b) a modified data\CompiledDefs\game.bin whose CHeroMorphDef entries
        (10535 AND 11246 -- owner unresolved, both carry identical retail
        TextureMorphs, so both are edited) gain one 24-byte TextureMorphs
        record per trigger stat:
            {u32 stat, u32 stat, u32 layer, u32 baseTexId, u32 newTexId, u32 2}

OVERLAY FORMAT SPEC (retail census, 235 blending overlays):
    region  base sheet          id    overlay dims  fmt   transp mips
    face    HERO_SKIN_FACE      1250  512x512       DXT3  1      8
    arms    HERO_SKIN_ARMS      1248  512x512       DXT3  1      8
    torso   HERO_SKIN_TORSO     1252  512x512       DXT3  1      8
    back    HERO_SKIN_BACK      1249  512x512       DXT3  1      8
    legs    HERO_SKIN_LEGS      1251  512x512       DXT3  1      8
    mouth   HERO_SKIN_MOUTH     1253  256x256       DXT1* 0      7
  (*retail mouth overlays are opaque DXT1; this tool still emits DXT3/1 for
   mouth glyphs so they can blend -- untested in-engine, see doc gaps.)
  Overlay UV layout == base sheet UV layout (same mesh UVs sample both).

TRIGGER STATS (morph-stat indices observed across the 156 retail records):
    stubble=3  hair_age=4  age=5  evil=6  good=7  physique=8  fatness=9
    will=10  body_hair=11
  TextureMorphs records carry NO threshold floats (ParticleMorphs do:
  rank-to-appear/disappear), so overlay opacity is driven continuously by the
  stat's normalized rank at runtime. "ALWAYS-ON" therefore cannot be proven
  from one record; `--stat always` emits the SAME overlay under BOTH stat 6
  (evil) and stat 7 (good) -- the two halves of the alignment axis -- which is
  the best data-side approximation (a perfectly neutral hero may render it at
  reduced opacity; compositor RE is a flagged Ghidra follow-up). For a
  GUARANTEED always-on tattoo add --bake-base, which additionally composites
  the glyph straight into the base HERO_SKIN_* sheet (proven visible by the
  stage-2 smoke test).

USAGE:
  python tools/hero_overlay.py spec
  python tools/hero_overlay.py stage <overlay.png> --region face --stat always
        [--layer 4] [--name COMP_HERO_SKIN_FACE_MYMOD]
        [--out work/hero_overlay/stage]
        [--textures-big PATH]   default: ledgered pristine backup
        [--game-root ROOT]      default: retail install (READ ONLY)
        [--defs 10535,11246] [--bake-base] [--no-forge]

Everything is validated by decode-back before the stage is declared good:
new texture decodes (PSNR vs source), every untouched textures.big entry is
byte-identical, every untouched game.bin entry is byte-identical, the edited
defs re-parse (count/record/tail check) and -- when forge.exe + the def schema
are present -- `forge defs decode` reports leftover 0 on both entries.
"""
import hashlib
import json
import os
import struct
import subprocess
import sys
import zlib

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
if TOOLS not in sys.path:
    sys.path.insert(0, TOOLS)

import big_write
import parse_bigb
import texture_build

DEFAULT_GAME_ROOT = r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters'
DEFAULT_TEXTURES = os.path.join(
    ROOT, r'work\smoke_test\backups\data\graphics\pc\textures.big')
PRISTINE_TEXTURES_SHA = \
    '39ef83511b95109f41895d608e757bbbdd9e5128730a36e86b45abe9f23ddc78'
FORGE_EXE = r'D:\Code\FableForge\build\forge.exe'
DEF_SCHEMA = os.path.join(ROOT, r'ghidra_out\def_schema.json')

SUB = 'GBANK_MAIN_PC'
RETAIL_TEX_CRC = 0x7C8D23C4          # constant on every GBANK_MAIN_PC record
MORPHDEF_ENTRIES = (10535, 11246)    # both populated CHeroMorphDefs (unnamed)

REGIONS = {
    'face':  dict(base_id=1250, base='HERO_SKIN_FACE',  dim=512, mips=8),
    'arms':  dict(base_id=1248, base='HERO_SKIN_ARMS',  dim=512, mips=8),
    'torso': dict(base_id=1252, base='HERO_SKIN_TORSO', dim=512, mips=8),
    'back':  dict(base_id=1249, base='HERO_SKIN_BACK',  dim=512, mips=8),
    'legs':  dict(base_id=1251, base='HERO_SKIN_LEGS',  dim=512, mips=8),
    'mouth': dict(base_id=1253, base='HERO_SKIN_MOUTH', dim=256, mips=7),
}
STATS = {'stubble': 3, 'hair_age': 4, 'age': 5, 'evil': 6, 'good': 7,
         'physique': 8, 'fatness': 9, 'will': 10, 'body_hair': 11}


def crc0(s):
    """Fable def field tag: reflected CRC-32, seed 0, no final xor."""
    crc = 0
    for ch in s.encode('latin1'):
        crc ^= ch
        for _ in range(8):
            crc = (crc >> 1) ^ (0xEDB88320 if crc & 1 else 0)
    return crc


TEXMORPH_TAG = struct.pack('<I', crc0('TextureMorphs'))     # de 9b 05 16


# ===========================================================================
# game.bin container R/W (mirror of FableForge forge::bin -- bin.cpp).
# The main table (names refs) is preserved VERBATIM: payload edits only,
# so names.bin never changes. Chunks are re-cut at <=32 KiB inflated
# (same rule as forge::bin::File::save) and zlib-recompressed.
# ===========================================================================
class GameBin:
    def __init__(self, path):
        self.path = path
        b = open(path, 'rb').read()
        if b[1] == 0xAA:
            raise ValueError('Xbox-format bin not supported')
        self.header = bytes(b[:9])
        self.count = struct.unpack_from('<I', b, 9)[0]
        main = 13
        self.main_table = bytes(b[main:main + self.count * 12])
        ccpos = main + self.count * 12
        chunk_count = struct.unpack_from('<I', b, ccpos)[0]
        ctab = ccpos + 4
        data_off = ctab + chunk_count * 8 + 4
        self.payloads = [b''] * self.count
        for c in range(1, chunk_count):
            fe, cs = struct.unpack_from('<ii', b, ctab + (c - 1) * 8)
            ee, ce = struct.unpack_from('<ii', b, ctab + c * 8)
            if ce - cs <= 0 or ee - fe <= 0:
                continue
            infl = zlib.decompress(b[data_off + cs:data_off + ce])
            n = ee - fe
            for local in range(n):
                start = struct.unpack_from('<H', infl, local * 2)[0]
                end = len(infl)
                if local + 1 < n:
                    end = struct.unpack_from('<H', infl, (local + 1) * 2)[0]
                self.payloads[fe + local] = bytes(infl[start:end])

    def serialize(self):
        """Rebuild the container bytes (forge::bin::File::save chunking)."""
        max_chunk = 32768
        chunks, starts, offs = [], [], []
        total = 0
        cur_off, cur_pay, first = [], bytearray(), 0

        def flush(end_index):
            nonlocal cur_off, cur_pay, first, total
            if not cur_off:
                return
            head = len(cur_off) * 2
            raw = bytearray()
            for o in cur_off:
                raw += struct.pack('<H', head + o)
            raw += cur_pay
            comp = zlib.compress(bytes(raw))
            starts.append(end_index - len(cur_off))
            offs.append(total)
            total += len(comp)
            chunks.append(comp)
            cur_off, cur_pay = [], bytearray()

        for i, pay in enumerate(self.payloads):
            projected = (len(cur_off) + 1) * 2 + len(cur_pay) + len(pay)
            if projected > max_chunk:
                flush(i)
            cur_off.append(len(cur_pay))
            cur_pay += pay
        flush(self.count)

        out = bytearray(self.header)
        out += struct.pack('<I', self.count)
        out += self.main_table
        out += struct.pack('<I', len(chunks) + 1)
        for s, o in zip(starts, offs):
            out += struct.pack('<II', s, o)
        out += struct.pack('<III', self.count, total, total)
        for c in chunks:
            out += c
        return bytes(out)


def insert_texture_morphs(payload, recs):
    """Append 24-byte records to the entry's TextureMorphs vector."""
    p = payload.find(TEXMORPH_TAG)
    if p < 0:
        raise ValueError('TextureMorphs tag not found in def payload')
    cnt = struct.unpack_from('<I', payload, p + 4)[0]
    start = p + 8
    end = start + cnt * 24
    added = b''.join(struct.pack('<6I', *r) for r in recs)
    return (payload[:p + 4] + struct.pack('<I', cnt + len(recs)) +
            payload[start:end] + added + payload[end:])


# ===========================================================================
# staging
# ===========================================================================
def _sha256(data):
    return hashlib.sha256(data).hexdigest()


def _parse_big(buf):
    _, _, fo, _ = parse_bigb.parse_header(buf)
    subs, _ = parse_bigb.parse_footer(buf, fo)
    parsed = []
    for s in subs:
        entries, _, _, err = parse_bigb.parse_toc(buf, s, fo)
        if err:
            raise ValueError('TOC parse error in %s: %s' % (s['name'], err))
        parsed.append((s, entries))
    return parsed


def stage(png_path, region, stats, layer=4, name=None, out_dir=None,
          textures_big=DEFAULT_TEXTURES, game_root=DEFAULT_GAME_ROOT,
          defs=MORPHDEF_ENTRIES, bake_base=False, run_forge=True, log=print):
    reg = REGIONS[region]
    out_dir = out_dir or os.path.join(ROOT, r'work\hero_overlay\stage')
    name = name or 'COMP_HERO_SKIN_%s_CUSTOM' % region.upper()
    val_dir = os.path.join(os.path.dirname(out_dir.rstrip('\\/')), 'validate')
    os.makedirs(val_dir, exist_ok=True)

    report = dict(overlay_png=os.path.abspath(png_path), region=region,
                  entry_name=name, layer=layer, stats=stats,
                  defs_edited=list(defs), bake_base=bake_base, checks=[])

    def check(label, ok, detail=''):
        report['checks'].append(dict(label=label, ok=bool(ok), detail=detail))
        log('  [%s] %s %s' % ('OK' if ok else 'FAIL', label, detail))
        if not ok:
            raise SystemExit('validation failed: ' + label)

    # ---- 1. overlay texture entry -----------------------------------------
    log('== overlay texture ==')
    rgba = texture_build.load_image_rgba(png_path)
    rgba = texture_build.fit_to_alloc(rgba, reg['dim'], reg['dim'])
    payload, info = texture_build.build_entry(
        rgba, texture_build.FMT_DXT3, mips=reg['mips'], transparency=1)

    src_buf = open(textures_big, 'rb').read()
    src_sha = _sha256(src_buf)
    if src_sha != PRISTINE_TEXTURES_SHA:
        log('  note: source textures.big is not the ledgered pristine retail '
            'copy (sha %s...)' % src_sha[:12])
    src_parsed = _parse_big(src_buf)
    main = next(e for s, e in src_parsed if s['name'] == SUB)
    if any(e['name'] == name for e in main):
        raise SystemExit('entry name %r already exists in %s' % (name, SUB))
    new_id = max(e['id'] for e in main) + 1
    report['new_texture'] = dict(id=new_id, sub=SUB, fmt='DXT3',
                                 transparency=1,
                                 dims='%dx%d' % (reg['dim'], reg['dim']),
                                 mips=reg['mips'])

    edits = {}
    buf_for_rebuild = src_buf
    if bake_base:
        # composite the glyph straight into the base sheet (guaranteed-visible
        # path, stage-2-proven), preserving the base entry's own format/Info.
        be = next(e for e in main if e['id'] == reg['base_id'])
        bpay = src_buf[be['offset']:be['offset'] + be['size']]
        bd, bmips = texture_build.decode_entry(be['info'], bpay, max_mips=1)
        base_rgba = bmips[0].astype('float32')
        a = rgba[..., 3:4].astype('float32') / 255.0
        base_rgba[..., :3] = rgba[..., :3] * a + base_rgba[..., :3] * (1 - a)
        import numpy as np
        baked = base_rgba.clip(0, 255).astype('uint8')
        bpayload, binfo = texture_build.build_entry(
            baked, bd['fmt'], mips=bd['mips'], real_w=bd['width'],
            real_h=bd['height'], transparency=bd['transp'])
        ba = bytearray(src_buf)
        ioff = texture_build.info_abs_offset(src_buf, be)
        ba[ioff:ioff + 34] = binfo
        buf_for_rebuild = bytes(ba)
        edits[(SUB, be['name'])] = bpayload
        report['baked_base'] = reg['base']

    out_big = big_write.rebuild(
        buf_for_rebuild, edits=edits,
        adds=[dict(sub=SUB, name=name, payload=payload, type=0, info=info,
                   id=new_id, crc=RETAIL_TEX_CRC)])

    # decode-back validation
    new_parsed = _parse_big(out_big)
    new_main = next(e for s, e in new_parsed if s['name'] == SUB)
    ne = next(e for e in new_main if e['name'] == name)
    check('new entry present', ne['id'] == new_id,
          'id=%d type=%d' % (ne['id'], ne['type']))
    d, mips_out = texture_build.decode_entry(
        ne['info'], out_big[ne['offset']:ne['offset'] + ne['size']])
    p = texture_build.psnr(rgba, mips_out[0])
    check('overlay decodes back', d['fmt'] == texture_build.FMT_DXT3
          and d['transp'] == 1 and d['mips'] == reg['mips']
          and d['consumed'] == ne['size'],
          'DXT3 tt=%d mips=%d PSNR=%.2f dB' % (d['transp'], d['mips'], p))
    check('overlay PSNR sane', p > 25.0, '%.2f dB' % p)
    texture_build.save_png(mips_out[0], os.path.join(val_dir, name + '_decoded.png'))

    # untouched-entry census (byte compare via offset/size into both buffers)
    total = same = 0
    touched = {name} | set(k[1] for k in edits)
    for (s0, e0s), (s1, e1s) in zip(src_parsed, new_parsed):
        by1 = {e['name']: e for e in e1s}
        for e0 in e0s:
            if s0['name'] == SUB and e0['name'] in touched:
                continue
            e1 = by1[e0['name']]
            total += 1
            if src_buf[e0['offset']:e0['offset'] + e0['size']] == \
               out_big[e1['offset']:e1['offset'] + e1['size']]:
                same += 1
    check('untouched texture entries byte-identical',
          same == total, '%d/%d' % (same, total))

    # ---- 2. game.bin def edit ---------------------------------------------
    log('== CHeroMorphDef edit ==')
    src_bin_path = os.path.join(game_root, 'data', 'CompiledDefs', 'game.bin')
    gb = GameBin(src_bin_path)
    recs = [(st, st, layer, reg['base_id'], new_id, 2) for st in stats]
    report['records'] = [list(r) for r in recs]
    originals = {}
    for idx in defs:
        pay = gb.payloads[idx]
        if pay.find(TEXMORPH_TAG) < 0:
            raise SystemExit('entry %d has no TextureMorphs field' % idx)
        originals[idx] = pay
        gb.payloads[idx] = insert_texture_morphs(pay, recs)
    out_bin = gb.serialize()

    tmp_bin = os.path.join(val_dir, 'game.bin')
    open(tmp_bin, 'wb').write(out_bin)
    gb2 = GameBin(tmp_bin)
    check('game.bin entry count preserved', gb2.count == gb.count,
          str(gb2.count))
    diff = [i for i in range(gb.count)
            if gb2.payloads[i] != (originals.get(i) or gb.payloads[i])
            and i not in defs]
    same_edits = all(gb2.payloads[i] == gb.payloads[i] for i in defs)
    check('untouched def entries byte-identical', not diff,
          'first diff: %s' % diff[:3] if diff else '%d entries' % gb.count)
    check('edited payloads survive re-chunking', same_edits, '')
    for idx in defs:
        pay = gb2.payloads[idx]
        p = pay.find(TEXMORPH_TAG)
        cnt = struct.unpack_from('<I', pay, p + 4)[0]
        old_cnt = struct.unpack_from('<I', originals[idx], p + 4)[0]
        tail_old = originals[idx][p + 8 + old_cnt * 24:]
        tail_new = pay[p + 8 + cnt * 24:]
        appended = pay[p + 8 + old_cnt * 24:p + 8 + cnt * 24]
        want = b''.join(struct.pack('<6I', *r) for r in recs)
        check('entry %d records %d->%d, tail preserved' % (idx, old_cnt, cnt),
              cnt == old_cnt + len(recs) and appended == want
              and tail_old == tail_new, '')

    # ---- 3. write the stage ------------------------------------------------
    tex_rel = os.path.join('data', 'graphics', 'pc', 'textures.big')
    bin_rel = os.path.join('data', 'CompiledDefs', 'game.bin')
    names_rel = os.path.join('data', 'CompiledDefs', 'names.bin')
    for rel, data in ((tex_rel, out_big), (bin_rel, out_bin)):
        dst = os.path.join(out_dir, rel)
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        open(dst, 'wb').write(data)
        log('  wrote %s (%d B)' % (dst, len(data)))
    # unmodified names.bin copy makes the stage a self-contained defs root
    names_src = os.path.join(game_root, 'data', 'CompiledDefs', 'names.bin')
    dst = os.path.join(out_dir, names_rel)
    open(dst, 'wb').write(open(names_src, 'rb').read())

    # ---- 4. forge decode-back (leftover 0) ---------------------------------
    if run_forge and os.path.exists(FORGE_EXE) and os.path.exists(DEF_SCHEMA):
        for idx in defs:
            r = subprocess.run(
                [FORGE_EXE, 'defs', 'decode', out_dir, DEF_SCHEMA, str(idx)],
                capture_output=True, text=True)
            head = (r.stdout or r.stderr).splitlines()[0] if (r.stdout or r.stderr) else ''
            check('forge defs decode %d leftover 0' % idx,
                  r.returncode == 0 and 'leftover 0' in head, head.strip())
    else:
        log('  (forge.exe or schema missing -- skipped forge decode-back)')

    report['sha256'] = {tex_rel: _sha256(out_big), bin_rel: _sha256(out_bin)}
    mpath = os.path.join(out_dir, 'hero_overlay_manifest.json')
    json.dump(report, open(mpath, 'w'), indent=1)
    log('staged OK -> %s (manifest %s)' % (out_dir, mpath))
    log('NOT deployed. Deploy = copy the two staged files over the install '
        '(game closed, back up originals first).')
    return report


# ===========================================================================
# CLI
# ===========================================================================
def _opt(argv, key, default):
    if key in argv:
        return argv[argv.index(key) + 1]
    return default


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    cmd = sys.argv[1]
    argv = sys.argv[2:]
    if cmd == 'spec':
        print('region  base sheet        id    overlay format (retail census)')
        for r, m in REGIONS.items():
            print('%-7s %-17s %-5d %dx%d DXT3 transp=1 mips=%d'
                  % (r, m['base'], m['base_id'], m['dim'], m['dim'], m['mips']))
        print('stats:', ', '.join('%s=%d' % kv for kv in STATS.items()),
              '| always -> evil+good pair')
        return
    if cmd != 'stage':
        print(__doc__)
        return
    png = argv[0]
    region = _opt(argv, '--region', None)
    stat_arg = _opt(argv, '--stat', 'always')
    if region not in REGIONS:
        raise SystemExit('--region must be one of ' + '/'.join(REGIONS))
    stats = []
    for tok in stat_arg.split(','):
        tok = tok.strip().lower()
        if tok == 'always':
            stats += [STATS['evil'], STATS['good']]
        elif tok in STATS:
            stats.append(STATS[tok])
        else:
            stats.append(int(tok))
    defs = tuple(int(x) for x in
                 _opt(argv, '--defs', ','.join(map(str, MORPHDEF_ENTRIES))).split(','))
    stage(png, region, stats,
          layer=int(_opt(argv, '--layer', '4')),
          name=_opt(argv, '--name', None),
          out_dir=_opt(argv, '--out', None),
          textures_big=_opt(argv, '--textures-big', DEFAULT_TEXTURES),
          game_root=_opt(argv, '--game-root', DEFAULT_GAME_ROOT),
          defs=defs,
          bake_base='--bake-base' in argv,
          run_forge='--no-forge' not in argv)


if __name__ == '__main__':
    main()
