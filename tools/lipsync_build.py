#!/usr/bin/env python3
r"""Fable TLC dialogue.big LIPSYNC WRITER -- pairs with parse_bigb.decode_lipsync.

Grammar (uncompressed payload; EgoCore LipSyncParser.h confirms field-for-field,
already byte-exact-validated on all retail entries by parse_bigb):

  raw  := u32 visemeCount ;
          visemeCount x { u8 visemeID ; cstr mnemonic }        # retail: 2 chars
          u32 fps ;               # 43 = int(22050/512): one frame / 512 samples
          u32 frameCount ;
          frameCount x { u8 keyCount ; keyCount x { u8 visemeID ; u8 weight } }
  Info := f32 duration (4 bytes)                # frameCount == ceil(duration*fps)
  weight byte w = round-ish of weightFloat*255; visemeID indexes the dictionary;
  each viseme maps to a single-frame phoneme POSE anim (ANIM_*_PHONEME_*) blended
  by weight at runtime (docs/DEMON_DOOR_FACE.md).

CLI (always run against a COPY of retail .bigs, never originals):
  python tools/lipsync_build.py <dialogue.big> --roundtrip [N]
  python tools/lipsync_build.py <dialogue.big> --mod <out.big>   # edit+add test
  python tools/lipsync_build.py --verify-mod <out.big> [--orig <copy.big>]
"""
import math
import os
import struct
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)
import parse_bigb

FPS = 43                       # int(22050 / 512)


class LipSync(object):
    def __init__(self):
        self.visemes = []      # [(id:int, mnemonic:str)]
        self.fps = FPS
        self.frames = []       # [[(visemeId:int, weight:int 0..255)]]

    @property
    def duration(self):
        return self.frames and len(self.frames) / float(self.fps) or 0.0

    @staticmethod
    def decode(payload):
        """Uncompressed LIPSYNC payload -> LipSync. Raises on malformed."""
        ls = LipSync()
        c = 0
        (n,) = struct.unpack_from('<I', payload, c); c += 4
        for _ in range(n):
            vid = payload[c]; c += 1
            s = bytearray()
            while payload[c] != 0:
                s.append(payload[c]); c += 1
            c += 1
            ls.visemes.append((vid, s.decode('latin1')))
        ls.fps, kfcount = struct.unpack_from('<II', payload, c); c += 8
        for _ in range(kfcount):
            k = payload[c]; c += 1
            ls.frames.append([(payload[c + 2 * j], payload[c + 2 * j + 1])
                              for j in range(k)])
            c += 2 * k
        if c != len(payload):
            raise ValueError('tail bytes: %d' % (len(payload) - c))
        return ls

    def encode(self):
        out = bytearray()
        out += struct.pack('<I', len(self.visemes))
        for vid, mnem in self.visemes:
            out.append(vid)
            out += mnem.encode('latin1') + b'\x00'
        out += struct.pack('<II', self.fps, len(self.frames))
        for keys in self.frames:
            if len(keys) > 255:
                raise ValueError('>255 keys in one frame')
            out.append(len(keys))
            for vid, w in keys:
                out.append(vid & 0xFF)
                out.append(max(0, min(255, int(w))))
        return bytes(out)

    def info_blob(self):
        return struct.pack('<f', self.duration)


def synth_curve(duration, viseme_id=3, mnemonics=None, peak=1.0):
    """New-from-scratch curve: one viseme pulsing sinusoidally over `duration`
    (a 'talking mouth' placeholder for custom speech). Returns LipSync."""
    ls = LipSync()
    ls.visemes = mnemonics or [(0, 'AH'), (1, 'EE'), (2, 'MM'), (3, 'OH'), (4, 'SZ')]
    n = int(math.ceil(duration * FPS))
    for i in range(n):
        w = 0.5 * peak * (1.0 - math.cos(2 * math.pi * (i / float(FPS)) * 4.0))
        b = int(round(w * 255))
        ls.frames.append([(viseme_id, b)] if b > 0 else [])
    return ls


# ------------------------------------------------------------------ container

def load_lipsync_entries(path):
    b = open(path, 'rb').read()
    _m, _v, foot_off, _fs = parse_bigb.parse_header(b)
    subs, _ = parse_bigb.parse_footer(b, foot_off)
    entries = []
    for sub in subs:
        if not sub['name'].startswith('LIPSYNC'):
            continue
        es, _stats, _end, err = parse_bigb.parse_toc(b, sub, foot_off)
        if err:
            sys.stderr.write('TOC warn (%s): %s\n' % (sub['name'], err))
        for e in es:
            e['bank'] = sub['name']
            entries.append(e)
    return b, entries


def cmd_roundtrip(path, limit=None):
    b, entries = load_lipsync_entries(path)
    if limit:
        entries = entries[:limit]
    st = dict(n=0, empty=0, exact=0, diff=0, err=0,
              frames=0, keys=0, dur_ok=0, dur_off=0)
    diffs = []
    for e in entries:
        if e['size'] == 0:
            st['empty'] += 1
            continue
        payload = bytes(b[e['offset']:e['offset'] + e['size']])
        st['n'] += 1
        try:
            ls = LipSync.decode(payload)
        except Exception as ex:
            st['err'] += 1
            if len(diffs) < 10:
                diffs.append((e['name'], str(ex)))
            continue
        st['frames'] += len(ls.frames)
        st['keys'] += sum(len(f) for f in ls.frames)
        if ls.encode() == payload:
            st['exact'] += 1
        else:
            st['diff'] += 1
            if len(diffs) < 10:
                diffs.append((e['name'], 'byte diff'))
        if e['infosize'] == 4:
            dur = struct.unpack('<f', e['info'])[0]
            # frameCount tracks ceil(duration*fps)
            if abs(len(ls.frames) - dur * ls.fps) <= ls.fps:  # within 1 s slack
                st['dur_ok'] += 1
            else:
                st['dur_off'] += 1
    print('lipsync round-trip: %(exact)d/%(n)d byte-exact (diff %(diff)d, '
          'decode-err %(err)d, empty skipped %(empty)d)' % st)
    print('  frames %(frames)d, weight keys %(keys)d, '
          'Info-duration coherent %(dur_ok)d / off %(dur_off)d' % st)
    for d in diffs:
        print('  DIFF %s' % (d,))
    return 0 if (st['diff'] == 0 and st['err'] == 0) else 1


MOD_ADD_NAME = 'LIPSYNC_FORGE_TEST_OH'
WEIGHT_SCALE = 0.5


def _pick_donor(b, entries):
    for e in entries:
        if e['size'] > 64 and e['infosize'] == 4:
            return e
    raise RuntimeError('no donor entry')


def cmd_mod(src_path, out_path):
    b, entries = load_lipsync_entries(src_path)
    donor = _pick_donor(b, entries)
    payload = bytes(b[donor['offset']:donor['offset'] + donor['size']])
    ls = LipSync.decode(payload)
    assert ls.encode() == payload

    # --- edit: halve every mouth weight (mumbling) ---
    half = LipSync.decode(payload)
    half.frames = [[(vid, int(w * WEIGHT_SCALE)) for vid, w in fr]
                   for fr in half.frames]
    # --- add: synthetic 2.0 s curve using the donor's viseme dictionary ---
    synth = synth_curve(2.0, viseme_id=ls.visemes[0][0] if ls.visemes else 0,
                        mnemonics=ls.visemes)

    import big_write
    out = big_write.rebuild(
        b,
        edits={(donor['bank'], donor['name']): half.encode()},
        adds=[dict(sub=donor['bank'], name=MOD_ADD_NAME, payload=synth.encode(),
                   type=donor['type'], info=synth.info_blob(),
                   deps=donor['deps'], crc=donor['crc'])])
    with open(out_path, 'wb') as f:
        f.write(out)
    print('wrote %s (%d B; edit %r weights x%g, add %r %.2fs %d frames, '
          'deps %s)' % (out_path, len(out), donor['name'], WEIGHT_SCALE,
                        MOD_ADD_NAME, synth.duration, len(synth.frames),
                        donor['deps']))
    print('donor bank=%s type=%d' % (donor['bank'], donor['type']))
    return 0


def cmd_verify_mod(mod_path, orig_path=None):
    b, entries = load_lipsync_entries(mod_path)
    by_name = {e['name']: e for e in entries}
    ok = True

    e2 = by_name.get(MOD_ADD_NAME)
    if e2 is None:
        print('MISSING added entry %s' % MOD_ADD_NAME)
        return 1
    d2 = parse_bigb.decode_lipsync(b, e2)
    print('%s: %d visemes, fps=%d, %d frames, dur(Info)=%.4f, byte_exact=%s, '
          'speaker=%s' % (MOD_ADD_NAME, len(d2['visemes']), d2['marker'],
                          d2['kfcount'], d2['duration'], d2['byte_exact'],
                          d2['speaker']))
    ok &= d2['byte_exact'] and d2['marker'] == FPS
    ok &= abs(d2['kfcount'] - d2['duration'] * FPS) < 1.0

    if orig_path:
        ob, oentries = load_lipsync_entries(orig_path)
        oby = {e['name']: e for e in oentries}
        donor = _pick_donor(ob, oentries)
        od = parse_bigb.decode_lipsync(ob, oby[donor['name']])
        nd = parse_bigb.decode_lipsync(b, by_name[donor['name']])
        wpairs = [(nw, ow) for nf, of in zip(nd['keyframes'], od['keyframes'])
                  for (_, nw), (_, ow) in zip(nf, of)]
        bad = [p for p in wpairs if p[0] != int(p[1] * WEIGHT_SCALE)]
        print('%s: %d weights checked, %d mismatched (want 0)' %
              (donor['name'], len(wpairs), len(bad)))
        ok &= not bad and nd['byte_exact']
        same = 0
        for name, oe in oby.items():
            if name == donor['name']:
                continue
            ne = by_name[name]
            if bytes(b[ne['offset']:ne['offset'] + ne['size']]) == \
               bytes(ob[oe['offset']:oe['offset'] + oe['size']]):
                same += 1
        print('untouched entries byte-identical: %d/%d' % (same, len(oby) - 1))
        ok &= (same == len(oby) - 1)
    print('VERIFY %s' % ('PASS' if ok else 'FAIL'))
    return 0 if ok else 1


def main():
    argv = sys.argv[1:]
    if '--verify-mod' in argv:
        mod = argv[argv.index('--verify-mod') + 1]
        orig = argv[argv.index('--orig') + 1] if '--orig' in argv else None
        return cmd_verify_mod(mod, orig)
    if not argv:
        print(__doc__)
        return 1
    path = argv[0]
    if '--roundtrip' in argv:
        i = argv.index('--roundtrip')
        n = int(argv[i + 1]) if i + 1 < len(argv) and argv[i + 1].isdigit() else None
        return cmd_roundtrip(path, n)
    if '--mod' in argv:
        return cmd_mod(path, argv[argv.index('--mod') + 1])
    print(__doc__)
    return 1


if __name__ == '__main__':
    sys.exit(main())
