#!/usr/bin/env python3
"""3DAF skeletal-animation parser for Fable: TLC graphics.big (ANIM_* entries).

Pure byte-level, NO Ghidra. Depends read-only on tools/parse_bigb.py.

WHAT THIS TOOL DOES (and honestly does NOT do)
----------------------------------------------
graphics.big MBANK Types 6/7/9 (`ANIM_*`, 3435 entries) are Big Blue Box **3DAF**
("3D Animation File") images -- the *compiled/packed* runtime form produced by
`C3DAnimFileXLoader` in Fable.exe (RTTI classes confirmed in the binary; see
docs/BIG_ANIM_FORMAT.md).

The 3DAF image has a **plain, self-describing FourCC-chunk header** that this tool
DECODES and VALIDATES self-consistently across all retail entries:
    3DAF  copyright banner
    ANRT  root chunk  -> total playback DURATION (f32)         [DECODED, validated]
    AOBJ  animated-object chunk -> target RIG name             [DECODED]
    XSEQ  sequence chunk -> per-bone keyframe tracks           [COMPRESSED/PACKED -- NOT decoded]
    HLPR / MVEC / TMEV / AMSK / XALO  helper/movement/timing/mask/alloc chunks

The **XSEQ keyframe payload is a packed stream** (Big Blue Box CBYTE-varint framing +
a `0x000d7ada` format magic; the disk image is 1.02-5.09x smaller than the header's
`decompSize`). Bone names (3ds-Max Biped: "Scene Root","Bip01 Pelvis","Spine",
"Neck","Head","L Clavicle","L UpperArm","L Thigh","L Calf",...) appear as CLEARTEXT
FRAGMENTS inside XSEQ, but the quaternion rotation keys / translation keys / key times
are interleaved through the CBYTE framing and are NOT recoverable by byte pattern-scan
(same situation as the mesh geometry: only the deterministic C3DAnimFileXSequenceChunk
deserializer computes the byte offsets). Decoding the keyframe tracks is a scoped
Ghidra follow-up -- see docs/BIG_ANIM_FORMAT.md sec.6.

IMPORTANT: a *prior* version of this file claimed the XSEQ body was LZO1X-compressed
and that fps==30. Both are REFUTED by the retail bytes (LZO1X decode fails on every
entry; no f32 30.0 exists anywhere in the bank). Those claims were removed.

Usage:
  python parse_anim.py <graphics.big>                 # enumerate + validate header layer
  python parse_anim.py <graphics.big> --entry NAME    # dump one animation's decoded header
  python parse_anim.py <graphics.big> --validate N    # deep self-consistency over N entries
"""
import struct, sys, os, re, collections

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_bigb
from parse_bigb import parse_header, parse_footer

ANIM_TYPES = (6, 7, 9)
VERSION_CONST = 0x00403e12          # dword at payload[4:8], constant across the whole bank
XSEQ_MAGIC = b'\xda\x7a\x0d\x00'    # 0x000d7ada, appears just inside the XSEQ chunk

# 3ds-Max Biped bone-name fragments used to *count* animated bones (they survive as
# cleartext inside the packed XSEQ stream). This is a lower-bound identification, not a
# full per-bone track decode.
BIPED_BONES = [b'Scene Root', b'Pelvis', b'Spine', b'Neck', b'Head', b'Clavicle',
               b'UpperArm', b'Forearm', b'Hand', b'Finger', b'Thigh', b'Calf',
               b'Foot', b'Toe', b'Ponytail', b'Tail', b'Bip01',
               b'Movement_dummy', b'Sub_m']


def load_bigb(path):
    with open(path, 'rb') as f:
        b = f.read()
    magic, ver, fo, fs = parse_header(b)
    if magic != b'BIGB':
        raise ValueError("not BIGB")
    subs, _ = parse_footer(b, fo)
    ents = []
    for sub in subs:
        es, st, ep, err = parse_bigb.parse_toc(b, sub, fo)
        for e in es:
            e['sub'] = sub['name']
            ents.append(e)
    return b, ents


def anim_entries(ents):
    return [e for e in ents if e['type'] in ANIM_TYPES]


# --------------------------------------------------------------------------
# 3DAF header decode (the recoverable, plain layer)
# --------------------------------------------------------------------------
def decode_header(pay):
    """Decode the plain 3DAF chunk header. Returns a dict. All fields are read from
    self-describing FourCC anchors, so this is robust and validated (see validate())."""
    r = dict()
    r['decomp_size'] = struct.unpack_from('<I', pay, 0)[0] if len(pay) >= 4 else None
    r['version'] = struct.unpack_from('<I', pay, 4)[0] if len(pay) >= 8 else None
    r['payload_size'] = len(pay)

    i3 = pay.find(b'3DAF'); r['i3daf'] = i3
    ia = pay.find(b'ANRT'); r['ianrt'] = ia
    # ANRT: FourCC + CBYTE framing then f32 total duration. Empirically the duration f32
    # sits at ANRT+9 (validated == TOC Info[0] on 3141/3285 entries with nonzero Info).
    r['duration'] = None
    if ia >= 0 and ia + 13 <= len(pay):
        r['duration'] = struct.unpack_from('<f', pay, ia + 9)[0]

    # AOBJ: animated-object chunk -> target rig name (cleartext ascii run after framing)
    io = pay.find(b'AOBJ'); r['iaobj'] = io
    r['rig'] = None
    if io >= 0:
        m = re.search(rb'[A-Za-z][A-Za-z0-9_]{3,40}', pay[io + 4:io + 64])
        if m:
            # trailing CBYTE-framing byte can bleed onto the last char; report the run raw.
            r['rig'] = m.group().decode('latin1')

    ix = pay.find(b'XSEQ'); r['ixseq'] = ix
    r['xseq_magic'] = (ix >= 0 and XSEQ_MAGIC in pay[ix:ix + 12])

    # chunk presence map
    r['chunks'] = [t.decode() for t in (b'3DAF', b'ANRT', b'AOBJ', b'XSEQ', b'HLPR',
                                        b'MVEC', b'TMEV', b'AMSK', b'XALO') if t in pay]
    return r


def bone_fragments(pay):
    """Count animated-bone name fragments visible in the (packed) XSEQ region.
    Returns list of (name, count). Lower bound on the animated bone set."""
    ix = pay.find(b'XSEQ')
    reg = pay[ix:] if ix >= 0 else pay
    out = []
    for nm in BIPED_BONES:
        c = reg.count(nm)
        if c:
            out.append((nm.decode(), c))
    return out


# --------------------------------------------------------------------------
def cmd_all(b, ents):
    anims = anim_entries(ents)
    print("=== ANIM_* (3DAF) entries in graphics.big ===")
    byt = collections.Counter(e['type'] for e in anims)
    print("  count=%d  by type=%s" % (len(anims), dict(sorted(byt.items()))))
    crc = collections.Counter(e['crc'] for e in anims)
    print("  CRC classes: %s" % {("0x%08x" % k): v for k, v in crc.most_common()})

    ver_ok = hdr_order = dur_ok = dur_tot = rig_ok = magic_ok = 0
    ratios = []
    for e in anims:
        pay = b[e['offset']:e['offset'] + e['size']]
        h = decode_header(pay)
        if h['version'] == VERSION_CONST:
            ver_ok += 1
        if h['i3daf'] >= 0 and h['ianrt'] > h['i3daf']:
            hdr_order += 1
        if h['rig']:
            rig_ok += 1
        if h['xseq_magic']:
            magic_ok += 1
        if h['decomp_size']:
            ratios.append(h['decomp_size'] / max(h['payload_size'], 1))
        if e['infosize'] >= 4:
            i0 = struct.unpack_from('<f', e['info'], 0)[0]
            if i0 != 0:
                dur_tot += 1
                if h['duration'] is not None and abs(h['duration'] - i0) < 1e-3:
                    dur_ok += 1
    N = len(anims)
    ratios.sort()
    print("  version == 0x%08x        : %d/%d" % (VERSION_CONST, ver_ok, N))
    print("  3DAF before ANRT (order)  : %d/%d" % (hdr_order, N))
    print("  AOBJ rig name recovered   : %d/%d" % (rig_ok, N))
    print("  ANRT duration == Info[0]  : %d/%d (nonzero-Info entries)" % (dur_ok, dur_tot))
    print("  XSEQ magic 0x000d7ada     : %d/%d" % (magic_ok, N))
    if ratios:
        print("  decompSize/paysize ratio  : min=%.2f med=%.2f max=%.2f  (XSEQ is PACKED)"
              % (ratios[0], ratios[len(ratios) // 2], ratios[-1]))
    print("  NOTE: XSEQ keyframe tracks (quat/trans/time) are a packed stream -- NOT decoded")
    print("        here (Ghidra follow-up: C3DAnimFileXSequenceChunk). See BIG_ANIM_FORMAT.md.")


def cmd_entry(b, ents, name):
    e = next((x for x in anim_entries(ents) if x['name'] == name), None)
    if not e:
        print("no ANIM entry named %r" % name); return
    pay = b[e['offset']:e['offset'] + e['size']]
    h = decode_header(pay)
    print("=== %s (type=%d size=%d crc=0x%08x) ===" % (name, e['type'], e['size'], e['crc']))
    print("  decompSize=%s  paysize=%d  ratio=%.2fx  version=0x%08x"
          % (h['decomp_size'], h['payload_size'],
             (h['decomp_size'] / max(h['payload_size'], 1)) if h['decomp_size'] else 0,
             h['version'] or 0))
    print("  duration=%.4f s (ANRT)  target rig=%r  XSEQ magic=%s"
          % (h['duration'] or 0, h['rig'], h['xseq_magic']))
    print("  chunks present: %s" % ', '.join(h['chunks']))
    if e['infosize'] >= 4:
        info = struct.unpack_from('<%df' % (e['infosize'] // 4), e['info'])
        print("  TOC Info floats: %s" % ['%.4f' % f for f in info])
    frags = bone_fragments(pay)
    print("  animated-bone name fragments in XSEQ (lower bound): %d distinct"
          % len(frags))
    for nm, c in frags:
        print("      %-16s x%d" % (nm, c))
    print("  [XSEQ per-bone keyframe tracks are packed -- not byte-decoded, see doc sec.6]")


def cmd_validate(b, ents, n):
    anims = anim_entries(ents)[:n]
    print("=== deep header validation over %d ANIM (3DAF) entries ===" % len(anims))
    dur_ok = dur_tot = 0
    for e in anims:
        pay = b[e['offset']:e['offset'] + e['size']]
        h = decode_header(pay)
        i0 = None
        if e['infosize'] >= 4:
            i0 = struct.unpack_from('<f', e['info'], 0)[0]
            if i0 != 0:
                dur_tot += 1
                if h['duration'] is not None and abs(h['duration'] - i0) < 1e-3:
                    dur_ok += 1
        frags = bone_fragments(pay)
        print("  %-42s rig=%-22s dur=%.3f Info0=%s bones>=%d magic=%s"
              % (e['name'][:42], str(h['rig'])[:22], h['duration'] or -1,
                 ('%.3f' % i0) if i0 is not None else '-', len(frags),
                 'Y' if h['xseq_magic'] else 'n'))
    print("--- totals ---")
    print("  ANRT duration == TOC Info[0]: %d/%d nonzero-Info entries" % (dur_ok, dur_tot))
    print("  (keyframe-track byte decode: PENDING Ghidra -- see docs/BIG_ANIM_FORMAT.md)")


def main():
    if len(sys.argv) < 2:
        print(__doc__); return
    path = sys.argv[1]
    b, ents = load_bigb(path)
    if '--entry' in sys.argv:
        cmd_entry(b, ents, sys.argv[sys.argv.index('--entry') + 1])
    elif '--validate' in sys.argv:
        cmd_validate(b, ents, int(sys.argv[sys.argv.index('--validate') + 1]))
    else:
        cmd_all(b, ents)


if __name__ == '__main__':
    main()
