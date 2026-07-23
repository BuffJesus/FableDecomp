#!/usr/bin/env python3
r"""Fable TLC ANIM (3DAF) WRITER -- the write path pairing parse_anim_xseq.py.

Grammar source: EgoCore AnimParser.h/AnimCompiler.h (answer key, ported -- see
docs/BIG_ANIM_FORMAT.md S9) + retail bytes. EgoCore's AnimCompiler is a
*semantic* writer (re-derives palettes, drops header bytes, reorders chunks);
this module is instead a LOSSLESS field-level model: decode->encode of the
decompressed 3DAF image is byte-exact on retail entries, which proves the
grammar is closed and makes surgical edits safe.

Model (decompressed image):

  image  := prefix(raw: 0x3E3E3E3E '3DAF' u32 <copyright cstr> pad4)
            { chunk }*  trailer(raw)
  chunk  := char[4] fourcc ; u32 size ; payload[size]
  Superchunks carry structured heads + nested chunks + raw tails:
    ANRT: u8 isCyclic ; f32 duration ; header(raw) ; children ; tail
    AOBJ: cstr rigName ; header(raw) ; children ; tail
    HLPR: children ; tail
    MVEC: f32[3] (raw12) ; children ; tail
  Leaf chunks (TMEV/AMSK/XALO/unknown) are kept raw.
  XSEQ/SEQ0 = one bone track (see Track). Any node whose structured re-encode
  does not reproduce its source bytes falls back to verbatim raw (counted).

Container framing (per BIG_ANIM_FORMAT.md S9 / HANDOFF):
  payload := u32 decompSize ; ONE raw LZO1X stream (lionhead_lz_compress),
  or the raw image itself if it already starts with 0x3E3E3E3E.
  NOTE: our LZO encoder is valid but NOT byte-identical to the retail
  compressor's choices, so the byte-exact proof lives at the image level;
  the container proof is decompress(compose(img)) == img.

CLI (always run against a COPY of retail .bigs, never originals):
  python tools/anim_build.py <graphics.big> --roundtrip [N]
  python tools/anim_build.py <graphics.big> --mod <out.big>   # mod+add test build
  python tools/anim_build.py --verify-mod <out.big>           # re-open + check
"""
import os
import struct
import sys
import time

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)
import lionhead_lz
import lionhead_lz_compress
import parse_bigb
import parse_anim_xseq

UNCOMP_MAGIC = 0x3E3E3E3E
SUPER = {b'ANRT', b'AOBJ', b'HLPR', b'MVEC'}
# fourccs the retail reader treats as chunk starts inside super-chunk headers
ANRT_STOPS = (b'HLPR', b'AOBJ', b'XALO')
AOBJ_STOPS = (b'XSEQ', b'SEQ0', b'AMSK')


# ------------------------------------------------------------------ nodes

class Node(object):
    """One chunk. kind in {'raw','anrt','aobj','hlpr','mvec','xseq'}."""

    def __init__(self, fourcc, kind):
        self.fourcc = fourcc          # bytes[4]
        self.kind = kind
        self.raw = None               # verbatim payload (leaf / fallback)
        self.children = []
        self.tail = b''               # unparsed bytes after children
        self.head = b''               # freeform header bytes (ANRT/AOBJ)
        # anrt
        self.is_cyclic = 0
        self.duration_raw = b'\x00' * 4
        # aobj
        self.name = ''
        # mvec
        self.vec_raw = b'\x00' * 12
        # xseq
        self.track = None

    # duration as float
    @property
    def duration(self):
        return struct.unpack('<f', self.duration_raw)[0]

    @duration.setter
    def duration(self, v):
        self.duration_raw = struct.pack('<f', v)

    def payload(self):
        if self.raw is not None:
            return self.raw
        out = bytearray()
        if self.kind == 'anrt':
            out.append(1 if self.is_cyclic else 0)
            out += self.duration_raw
            out += self.head
        elif self.kind == 'aobj':
            out += self.name.encode('latin1') + b'\x00'
            out += self.head
        elif self.kind == 'mvec':
            out += self.vec_raw
        elif self.kind == 'xseq':
            return self.track.encode()
        for ch in self.children:
            out += ch.encode()
        out += self.tail
        return bytes(out)

    def encode(self):
        p = self.payload()
        return self.fourcc + struct.pack('<I', len(p)) + p

    def find(self, fourcc, out=None):
        """All descendant nodes (incl. self) with this fourcc (bytes)."""
        if out is None:
            out = []
        if self.fourcc == fourcc:
            out.append(self)
        for ch in self.children:
            ch.find(fourcc, out)
        return out


class Track(object):
    """One XSEQ bone track. Numeric f32 fields are kept as raw 4-byte cells
    (bit-exact round-trip incl. any NaN payloads); int16 position keys are
    kept as ints (exact). Sections may be absent (has_* flags) to reproduce
    truncated retail tracks byte-exact."""

    def __init__(self):
        self.bone_index = 0
        self.parent_index = -1
        self.bone_name = ''
        self.pre_fps_flag = 0
        self.fps_raw = struct.pack('<f', 30.0)
        self.frame_count = 0
        self.post_frame_flags = b'\x00' * 4
        self.pos_factor_raw = struct.pack('<f', 1.0)
        self.scale_factor_raw = struct.pack('<f', 1.0)
        self.rot_raw = b''            # rotCount x 16B (f32[4] xyzw)
        self.rot_palette = []
        self.pos_int = []             # posCount x (i16,i16,i16)
        self.pos_palette = []
        self.has_rot = self.has_rotpal = self.has_pos = self.has_pospal = True

    # ---- float views -------------------------------------------------
    def _f32(self, raw):
        return struct.unpack('<f', raw)[0]

    @property
    def fps(self):
        return self._f32(self.fps_raw)

    @fps.setter
    def fps(self, v):
        self.fps_raw = struct.pack('<f', v)

    @property
    def position_factor(self):
        return self._f32(self.pos_factor_raw)

    @position_factor.setter
    def position_factor(self, v):
        self.pos_factor_raw = struct.pack('<f', v)

    @property
    def rot_keys(self):
        n = len(self.rot_raw) // 16
        return [struct.unpack_from('<4f', self.rot_raw, i * 16) for i in range(n)]

    @rot_keys.setter
    def rot_keys(self, quats):
        self.rot_raw = b''.join(struct.pack('<4f', *q) for q in quats)

    @property
    def pos_keys(self):
        f = self.position_factor
        return [(x * f, y * f, z * f) for x, y, z in self.pos_int]

    def set_pos_keys(self, points, factor=None):
        """Quantize float positions. factor=None auto-picks from max |coord|."""
        if factor is None:
            m = max((max(abs(c) for c in p) for p in points), default=0.0)
            factor = max(m / 32767.0, 1e-6)
        self.position_factor = factor
        f = self.position_factor      # after f32 quantization of the factor
        self.pos_int = [tuple(max(-32767, min(32767, int(round(c / f))))
                              for c in p) for p in points]

    # ---- codec ---------------------------------------------------------
    @staticmethod
    def decode(buf, start, end):
        t = Track()
        c = start
        if c + 8 > end:
            return None
        t.bone_index, t.parent_index = struct.unpack_from('<Ii', buf, c); c += 8
        s = bytearray()
        while c < end and buf[c] != 0:
            s.append(buf[c]); c += 1
        if c >= end:
            return None
        c += 1
        t.bone_name = s.decode('latin1')
        if c + 17 > end:
            return None
        t.pre_fps_flag = buf[c]; c += 1
        t.fps_raw = bytes(buf[c:c + 4]); c += 4
        (t.frame_count,) = struct.unpack_from('<I', buf, c); c += 4
        t.post_frame_flags = bytes(buf[c:c + 4]); c += 4
        t.pos_factor_raw = bytes(buf[c:c + 4]); c += 4
        t.scale_factor_raw = bytes(buf[c:c + 4]); c += 4

        t.has_rot = t.has_rotpal = t.has_pos = t.has_pospal = False
        rot_count = 0
        if c + 2 <= end:
            t.has_rot = True
            (rot_count,) = struct.unpack_from('<H', buf, c); c += 2
            need = rot_count * 16
            if c + need > end:
                return None
            t.rot_raw = bytes(buf[c:c + need]); c += need
        if c + 2 <= end:
            t.has_rotpal = True
            (n,) = struct.unpack_from('<H', buf, c); c += 2
            wide = rot_count > 255
            step = 2 if wide else 1
            if c + n * step > end:
                return None
            if wide:
                t.rot_palette = list(struct.unpack_from('<%dH' % n, buf, c))
            else:
                t.rot_palette = list(buf[c:c + n])
            c += n * step
        pos_count = 0
        if c + 2 <= end:
            t.has_pos = True
            (pos_count,) = struct.unpack_from('<H', buf, c); c += 2
            if c + pos_count * 6 > end:
                return None
            v = struct.unpack_from('<%dh' % (pos_count * 3), buf, c)
            t.pos_int = [tuple(v[i:i + 3]) for i in range(0, len(v), 3)]
            c += pos_count * 6
        if c + 2 <= end:
            t.has_pospal = True
            (n,) = struct.unpack_from('<H', buf, c); c += 2
            wide = pos_count > 255
            step = 2 if wide else 1
            if c + n * step > end:
                return None
            if wide:
                t.pos_palette = list(struct.unpack_from('<%dH' % n, buf, c))
            else:
                t.pos_palette = list(buf[c:c + n])
            c += n * step
        if c != end:                  # unconsumed bytes -> not our grammar
            return None
        return t

    def encode(self):
        out = bytearray()
        out += struct.pack('<Ii', self.bone_index & 0xFFFFFFFF, self.parent_index)
        out += self.bone_name.encode('latin1') + b'\x00'
        out.append(self.pre_fps_flag)
        out += self.fps_raw
        out += struct.pack('<I', self.frame_count)
        out += self.post_frame_flags
        out += self.pos_factor_raw
        out += self.scale_factor_raw
        rot_count = len(self.rot_raw) // 16
        if self.has_rot:
            out += struct.pack('<H', rot_count)
            out += self.rot_raw
        if self.has_rotpal:
            out += struct.pack('<H', len(self.rot_palette))
            if rot_count > 255:
                out += struct.pack('<%dH' % len(self.rot_palette), *self.rot_palette)
            else:
                out += bytes(self.rot_palette)
        pos_count = len(self.pos_int)
        if self.has_pos:
            out += struct.pack('<H', pos_count)
            for p in self.pos_int:
                out += struct.pack('<3h', *p)
        if self.has_pospal:
            out += struct.pack('<H', len(self.pos_palette))
            if pos_count > 255:
                out += struct.pack('<%dH' % len(self.pos_palette), *self.pos_palette)
            else:
                out += bytes(self.pos_palette)
        return bytes(out)


class Anim3DAF(object):
    def __init__(self):
        self.prefix = b''             # >>>> 3DAF header block (raw)
        self.chunks = []              # top-level Nodes
        self.trailer = b''            # bytes after the last valid chunk
        self.fallbacks = 0            # nodes kept raw because re-encode mismatched

    def serialize(self):
        return self.prefix + b''.join(n.encode() for n in self.chunks) + self.trailer

    def find(self, fourcc):
        out = []
        for n in self.chunks:
            n.find(fourcc, out)
        return out

    def tracks(self):
        """(node, Track) for every structured XSEQ/SEQ0, doc order."""
        return [(n, n.track) for n in self.find(b'XSEQ') + self.find(b'SEQ0')
                if n.track is not None]


# ------------------------------------------------------------------ parse

def _walk(buf, cursor, end, anim):
    """Generic chunk walk. Returns (nodes, tail_raw_bytes)."""
    nodes = []
    while cursor + 8 <= end:
        sig = bytes(buf[cursor:cursor + 4])
        (size,) = struct.unpack_from('<I', buf, cursor + 4)
        pstart = cursor + 8
        nxt = pstart + size
        if nxt > end:
            break
        nodes.append(_parse_node(buf, sig, pstart, nxt, anim))
        cursor = nxt
    return nodes, bytes(buf[cursor:end])


def _scan_stop(buf, c, end, stops):
    while c + 4 <= end:
        if bytes(buf[c:c + 4]) in stops:
            return c
        c += 1
    return end


def _parse_node(buf, sig, pstart, end, anim):
    src = bytes(buf[pstart:end])
    if sig == b'ANRT' and end - pstart >= 5:
        n = Node(sig, 'anrt')
        n.is_cyclic = buf[pstart]
        n.duration_raw = bytes(buf[pstart + 1:pstart + 5])
        hs = pstart + 5
        he = _scan_stop(buf, hs, end, ANRT_STOPS)
        n.head = bytes(buf[hs:he])
        n.children, n.tail = _walk(buf, he, end, anim)
    elif sig == b'AOBJ':
        n = Node(sig, 'aobj')
        c = pstart
        s = bytearray()
        while c < end and buf[c] != 0:
            s.append(buf[c]); c += 1
        if c < end:
            c += 1
        n.name = s.decode('latin1')
        he = _scan_stop(buf, c, end, AOBJ_STOPS)
        n.head = bytes(buf[c:he])
        n.children, n.tail = _walk(buf, he, end, anim)
    elif sig == b'HLPR':
        n = Node(sig, 'hlpr')
        n.children, n.tail = _walk(buf, pstart, end, anim)
    elif sig == b'MVEC' and end - pstart >= 12:
        n = Node(sig, 'mvec')
        n.vec_raw = bytes(buf[pstart:pstart + 12])
        n.children, n.tail = _walk(buf, pstart + 12, end, anim)
    elif sig in (b'XSEQ', b'SEQ0'):
        n = Node(sig, 'xseq')
        n.track = Track.decode(buf, pstart, end)
        if n.track is None:
            n.raw = src
            anim.fallbacks += 1
        return n
    else:
        n = Node(sig, 'raw')
        n.raw = src
        return n
    # is_cyclic byte fidelity: retail stores 0/1 but guard any other value
    if n.kind == 'anrt' and n.is_cyclic not in (0, 1):
        n.raw = src
        anim.fallbacks += 1
        return n
    if n.payload() != src:            # lossless guarantee, per node
        n.raw = src
        n.children = []
        anim.fallbacks += 1
    return n


def parse_image(img):
    """Decompressed 3DAF image -> Anim3DAF (lossless)."""
    anim = Anim3DAF()
    c = 0
    if len(img) >= 8 and struct.unpack_from('<I', img, 0)[0] == UNCOMP_MAGIC:
        c = 4
        if img[c:c + 4] == b'3DAF':
            c += 8
            while c < len(img) and img[c] != 0:
                c += 1
            c += 1
            c = (c + 3) & ~3
        else:
            c = 4                     # bare magic, chunks follow
    anim.prefix = bytes(img[:c])
    anim.chunks, anim.trailer = _walk(img, c, len(img), anim)
    return anim


# ------------------------------------------------------------------ container

def decompress_payload(payload):
    return parse_anim_xseq.decompress_payload(payload)


def compose_payload(img, compress=True):
    """3DAF image -> ANIM entry payload ([u32 decompSize][raw LZO1X stream])."""
    if not compress:
        return bytes(img)             # engine accepts images starting 0x3E3E3E3E
    return struct.pack('<I', len(img)) + lionhead_lz_compress.compress(img)


def new_image(prefix=None):
    """Fresh empty 3DAF header block for from-scratch authoring."""
    if prefix is None:
        p = bytearray()
        p += struct.pack('<I', UNCOMP_MAGIC)
        p += b'3DAF'
        p += struct.pack('<I', 100)
        p += b'Copyright Big Blue Box Studios Ltd.\x00'
        while len(p) % 4:
            p.append(0)
        prefix = bytes(p)
    a = Anim3DAF()
    a.prefix = prefix
    return a


def anim_info_blob(duration, non_looping=None, mvec=(0.0, 0.0, 0.0), rotation=0.0):
    """24-byte TOC Info blob (C3DAnimationInfo::Serialize)."""
    if non_looping is None:
        non_looping = duration
    return struct.pack('<2f3ff', duration, non_looping, *mvec, rotation)


# ------------------------------------------------------------------ CLI ops

def cmd_roundtrip(path, limit=None):
    b, entries = parse_anim_xseq.load_anim_entries(path)
    if limit:
        entries = entries[:limit]
    t0 = time.time()
    stats = dict(n=0, empty=0, img_exact=0, img_diff=0, lzo_ok=0, lzo_bad=0,
                 comp_byte_match=0, fallback_nodes=0, tracks=0,
                 src_comp=0, our_comp=0)
    diffs = []
    for e in entries:
        payload = b[e['offset']:e['offset'] + e['size']]
        if len(payload) < 8:
            stats['empty'] += 1
            continue
        img = decompress_payload(payload)
        anim = parse_image(img)
        out = anim.serialize()
        stats['n'] += 1
        stats['fallback_nodes'] += anim.fallbacks
        stats['tracks'] += len(anim.tracks())
        if out == bytes(img):
            stats['img_exact'] += 1
        else:
            stats['img_diff'] += 1
            if len(diffs) < 10:
                diffs.append(e['name'])
        comp = compose_payload(out)
        stats['src_comp'] += len(payload)
        stats['our_comp'] += len(comp)
        if bytes(decompress_payload(comp)) == bytes(img):
            stats['lzo_ok'] += 1
        else:
            stats['lzo_bad'] += 1
        if comp == bytes(payload):
            stats['comp_byte_match'] += 1
    dt = time.time() - t0
    print('anim_build round-trip over %d entries (%.1fs):' % (len(entries), dt))
    print('  image recompose byte-exact : %(img_exact)d/%(n)d  (diff %(img_diff)d, '
          'raw-fallback nodes %(fallback_nodes)d, tracks %(tracks)d)' % stats)
    print('  recompress->decompress==img: %(lzo_ok)d/%(n)d  (bad %(lzo_bad)d)' % stats)
    print('  compressed bytes == retail : %(comp_byte_match)d/%(n)d  '
          '(expected ~0; different LZO match choices)' % stats)
    print('  compressed size: retail %(src_comp)d B -> ours %(our_comp)d B' % stats)
    if diffs:
        print('  first diffs: %s' % diffs)
    return 0 if (stats['img_diff'] == 0 and stats['lzo_bad'] == 0) else 1


MOD_EDIT_NAME = 'ANIM_HERO_THROW'
MOD_ADD_NAME = 'ANIM_HERO_THROW_SLOW'
POS_SCALE = 1.25
FPS_SCALE = 0.5


def _entry_payload(b, e):
    return b[e['offset']:e['offset'] + e['size']]


def cmd_mod(src_path, out_path):
    """Build a test .big (from a COPY) with 1 edited + 1 added animation."""
    b, entries = parse_anim_xseq.load_anim_entries(src_path)
    by_name = {e['name']: e for e in entries}
    e = by_name[MOD_EDIT_NAME]
    img = decompress_payload(_entry_payload(b, e))
    anim = parse_image(img)
    assert anim.serialize() == bytes(img), 'source not lossless?'

    # --- edit: amplitude-scale (bigger motion), duration unchanged ---
    edited = parse_image(img)
    for _, t in edited.tracks():
        if t.pos_int:
            t.position_factor = t.position_factor * POS_SCALE
    edit_payload = compose_payload(edited.serialize())

    # --- add: retimed copy (half speed), new TOC entry with new Info ---
    slow = parse_image(img)
    for _, t in slow.tracks():
        if t.fps > 0:
            t.fps = t.fps * FPS_SCALE
    new_dur = 0.0
    for _, t in slow.tracks():
        if t.fps > 0 and t.frame_count:
            new_dur = max(new_dur, t.frame_count / t.fps)
    for n in slow.find(b'ANRT'):
        n.duration = new_dur
    add_payload = compose_payload(slow.serialize())
    old_info = e['info']
    mvec = struct.unpack_from('<3f', old_info, 8) if len(old_info) >= 20 else (0, 0, 0)
    rot = struct.unpack_from('<f', old_info, 20)[0] if len(old_info) >= 24 else 0.0
    info = anim_info_blob(new_dur, new_dur, mvec, rot)

    import big_write
    out = big_write.rebuild(
        b,
        edits={(e['bank'], MOD_EDIT_NAME): edit_payload},
        adds=[dict(sub=e['bank'], name=MOD_ADD_NAME, payload=add_payload,
                   type=e['type'], info=info, crc=e['crc'],
                   deps=e['deps'])])
    with open(out_path, 'wb') as f:
        f.write(out)
    print('wrote %s (%d B; edit %s posFactor x%g, add %s fps x%g dur %.4fs)' %
          (out_path, len(out), MOD_EDIT_NAME, POS_SCALE, MOD_ADD_NAME,
           FPS_SCALE, new_dur))
    return 0


def cmd_verify_mod(mod_path, orig_path=None):
    b, entries = parse_anim_xseq.load_anim_entries(mod_path)
    by_name = {e['name']: e for e in entries}
    ok = True

    e = by_name[MOD_EDIT_NAME]
    a = parse_anim_xseq.parse_anim(_entry_payload(b, e))
    v = parse_anim_xseq.validate_anim(a)
    print('%s: %d tracks, dur %.4f, validate %s' %
          (MOD_EDIT_NAME, len(a.tracks), a.duration, v))
    ok &= (v['pal_oob'] == 0 and v['tail_bytes'] == 0 and v['quat_bad'] == 0)

    e2 = by_name.get(MOD_ADD_NAME)
    if e2 is None:
        print('MISSING added entry %s' % MOD_ADD_NAME)
        return 1
    a2 = parse_anim_xseq.parse_anim(_entry_payload(b, e2))
    v2 = parse_anim_xseq.validate_anim(a2)
    fpss = sorted({t.fps for t in a2.tracks})
    info_dur = struct.unpack_from('<f', e2['info'])[0] if e2['infosize'] >= 4 else None
    print('%s: %d tracks, ANRT dur %.4f, Info dur %s, fps set %s, validate %s' %
          (MOD_ADD_NAME, len(a2.tracks), a2.duration, info_dur, fpss, v2))
    ok &= (v2['pal_oob'] == 0 and v2['tail_bytes'] == 0)
    ok &= abs(a2.duration - (info_dur or 0)) < 1e-4

    if orig_path:
        ob, oentries = parse_anim_xseq.load_anim_entries(orig_path)
        oby = {e['name']: e for e in oentries}
        oa = parse_anim_xseq.parse_anim(_entry_payload(ob, oby[MOD_EDIT_NAME]))
        # edited: positions scaled, quats identical, fps identical
        for t_new, t_old in zip(a.tracks, oa.tracks):
            if t_old.pos_pool:
                r = t_new.pos_pool[0][0] / t_old.pos_pool[0][0] \
                    if t_old.pos_pool[0][0] else None
            assert t_new.rot_pool == t_old.rot_pool
            assert t_new.fps == t_old.fps
        oa2 = parse_anim_xseq.parse_anim(_entry_payload(ob, oby[MOD_EDIT_NAME]))
        for t_new, t_old in zip(a2.tracks, oa2.tracks):
            assert abs(t_new.fps - t_old.fps * FPS_SCALE) < 1e-5
            assert t_new.frame_count == t_old.frame_count
        # every OTHER anim entry decompresses to identical images
        same = 0
        for name, oe in oby.items():
            if name == MOD_EDIT_NAME:
                continue
            ne = by_name[name]
            if oe['size'] == 0 and ne['size'] == 0:
                same += 1
                continue
            if bytes(_entry_payload(b, ne)) == bytes(_entry_payload(ob, oe)):
                same += 1
        print('untouched entries byte-identical: %d/%d' % (same, len(oby) - 1))
        ok &= (same == len(oby) - 1)
    print('VERIFY %s' % ('PASS' if ok else 'FAIL'))
    return 0 if ok else 1


def main():
    argv = sys.argv[1:]
    if '--verify-mod' in argv:
        i = argv.index('--verify-mod')
        mod = argv[i + 1]
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
