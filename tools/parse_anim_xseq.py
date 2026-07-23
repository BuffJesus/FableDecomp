#!/usr/bin/env python3
"""Fable TLC graphics.big ANIM_* (3DAF) full keyframe decoder.

Supersedes the "XSEQ is packed / needs Ghidra" verdict in docs/BIG_ANIM_FORMAT.md
S6: the answer key is EgoCore's AnimParser.h
(C:/Users/Cornelio/Documents/EgoCoreInspect/EgoCore-master/EgoCore/Animations/AnimParser.h).

Container truth (ported from EgoCore, validated against retail bytes):

  payload := u32 decompSize ; <ONE raw LZO1X stream>          # no [u16 clen] frame
                                                              # headers (that is the
                                                              # texture/mesh framing)
  If the first u32 is 0x3E3E3E3E (">>>>") the payload is already uncompressed.

  decompressed := 0x3E3E3E3E '3DAF' u32 <copyright cstr> pad-to-4
                  { chunk }*
  chunk := char[4] fourcc ; u32 payloadSize ; payload[payloadSize]
     ANRT: u8 isCyclic ; f32 duration ; <freeform header bytes> ; nested chunks
     AOBJ: cstr objectName(rig) ; <freeform header bytes> ; nested chunks
     HLPR: nested chunks (helper-object tracks)
     MVEC: f32[3] movementVector ; nested chunks
     TMEV: cstr eventName ; f32 time
     AMSK: bone-mask bit words
     XALO: allocation-size hint bytes
     XSEQ / SEQ0: ONE PER BONE TRACK:
         u32 boneIndex ; i32 parentIndex ; cstr boneName ;
         u8  preFPSFlag ;
         f32 samplesPerSecond ; u32 frameCount ; u8 postFrameFlags[4] ;
         f32 positionFactor ; f32 scalingFactor ;
         u16 rotCount    ; rotCount    x f32[4]  quaternion (x,y,z,w) key pool
         u16 palRotCount ; palRotCount x (u8 if rotCount<=255 else u16)  frame->pool
         u16 posCount    ; posCount    x i16[3]  position * positionFactor key pool
         u16 palPosCount ; palPosCount x (u8 if posCount<=255 else u16)  frame->pool

  Palette tables are keyframe reduction: frame i uses pool[pal[i]]; empty palette
  means pool is per-frame dense. Runtime interpolates (quat slerp / pos lerp).

The old parse_anim.py header work (ANRT+9 duration slot, "CBYTE framing",
"decompSize>payload => bespoke packing") was reading the COMPRESSED stream;
its S2-S5 anchors were LZO literal-run artifacts. This module reads the real thing.

Usage:
  python tools/parse_anim_xseq.py <graphics.big> --list [PATTERN]
  python tools/parse_anim_xseq.py <graphics.big> --entry NAME [--json out.json]
  python tools/parse_anim_xseq.py <graphics.big> --validate [N]
"""
import json
import math
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lionhead_lz
import parse_bigb

UNCOMP_MAGIC = 0x3E3E3E3E
ANIM_TYPES = (6, 7, 9)


# ---------------------------------------------------------------- container

def load_anim_entries(path):
    b = open(path, 'rb').read()
    magic, ver, foot_off, foot_size = parse_bigb.parse_header(b)
    subs, _ = parse_bigb.parse_footer(b, foot_off)
    entries = []
    for sub in subs:
        es, _stats, _end, err = parse_bigb.parse_toc(b, sub, foot_off)
        if err:
            sys.stderr.write('TOC warn (%s): %s\n' % (sub['name'], err))
        for e in es:
            if e['type'] in ANIM_TYPES:
                e['bank'] = sub['name']
                entries.append(e)
    return b, entries


def decompress_payload(payload):
    """[u32 decompSize][raw LZO1X stream] -> decompressed bytes."""
    if len(payload) < 8:
        return None
    magic = struct.unpack_from('<I', payload, 0)[0]
    if magic == UNCOMP_MAGIC:
        return payload
    return lionhead_lz.decompress(payload[4:], expected_len=magic)


# ---------------------------------------------------------------- 3DAF parse

class Track(object):
    __slots__ = ('bone_index', 'parent_index', 'bone_name', 'pre_fps_flag',
                 'fps', 'frame_count', 'post_frame_flags',
                 'position_factor', 'scaling_factor',
                 'rot_pool', 'rot_palette', 'pos_pool', 'pos_palette',
                 'is_helper', 'consumed', 'chunk_size')

    def rot_at(self, frame):
        if not self.rot_pool:
            return None
        i = frame
        if self.rot_palette:
            i = self.rot_palette[min(frame, len(self.rot_palette) - 1)]
        return self.rot_pool[min(i, len(self.rot_pool) - 1)]

    def pos_at(self, frame):
        if not self.pos_pool:
            return None
        i = frame
        if self.pos_palette:
            i = self.pos_palette[min(frame, len(self.pos_palette) - 1)]
        return self.pos_pool[min(i, len(self.pos_pool) - 1)]


class Anim(object):
    def __init__(self):
        self.is_cyclic = False
        self.duration = 0.0
        self.object_name = ''
        self.movement_vector = None
        self.time_events = []       # (name, time)
        self.bone_mask = None
        self.xalo = None
        self.tracks = []
        self.helper_tracks = []
        self.decomp_size = 0
        self.chunk_counts = {}


def _cstr(buf, c, end):
    s = bytearray()
    while c < end and buf[c] != 0:
        s.append(buf[c]); c += 1
    if c < end:
        c += 1
    return s.decode('latin1'), c


def parse_xseq(buf, start, end):
    t = Track()
    c = start
    if c + 8 > end:
        return None
    t.bone_index, t.parent_index = struct.unpack_from('<Ii', buf, c); c += 8
    t.bone_name, c = _cstr(buf, c, end)
    if c + 17 > end:
        return None
    t.pre_fps_flag = buf[c]; c += 1
    t.fps, t.frame_count = struct.unpack_from('<fI', buf, c); c += 8
    t.post_frame_flags = tuple(buf[c:c + 4]); c += 4
    t.position_factor, t.scaling_factor = struct.unpack_from('<ff', buf, c); c += 8

    t.rot_pool = []
    t.rot_palette = []
    t.pos_pool = []
    t.pos_palette = []

    if c + 2 <= end:
        (rot_count,) = struct.unpack_from('<H', buf, c); c += 2
        for _ in range(rot_count):
            if c + 16 > end:
                break
            t.rot_pool.append(struct.unpack_from('<4f', buf, c)); c += 16
    if c + 2 <= end:
        (n,) = struct.unpack_from('<H', buf, c); c += 2
        wide = len(t.rot_pool) > 255
        for _ in range(n):
            if c + (2 if wide else 1) > end:
                break
            if wide:
                (i,) = struct.unpack_from('<H', buf, c); c += 2
            else:
                i = buf[c]; c += 1
            t.rot_palette.append(i)
    if c + 2 <= end:
        (pos_count,) = struct.unpack_from('<H', buf, c); c += 2
        f = t.position_factor
        for _ in range(pos_count):
            if c + 6 > end:
                break
            ix, iy, iz = struct.unpack_from('<3h', buf, c); c += 6
            t.pos_pool.append((ix * f, iy * f, iz * f))
    if c + 2 <= end:
        (n,) = struct.unpack_from('<H', buf, c); c += 2
        wide = len(t.pos_pool) > 255
        for _ in range(n):
            if c + (2 if wide else 1) > end:
                break
            if wide:
                (i,) = struct.unpack_from('<H', buf, c); c += 2
            else:
                i = buf[c]; c += 1
            t.pos_palette.append(i)
    t.consumed = c - start
    t.chunk_size = end - start
    return t


def parse_chunk_block(buf, cursor, end, anim, is_helper):
    while cursor + 8 <= end:
        sig = buf[cursor:cursor + 4]
        (size,) = struct.unpack_from('<I', buf, cursor + 4)
        pstart = cursor + 8
        nxt = pstart + size
        if nxt > end:
            break
        key = sig.decode('latin1', 'replace')
        anim.chunk_counts[key] = anim.chunk_counts.get(key, 0) + 1
        if sig == b'ANRT':
            anim.is_cyclic = buf[pstart] != 0
            (anim.duration,) = struct.unpack_from('<f', buf, pstart + 1)
            ic = pstart + 5
            while ic + 4 <= nxt:
                if buf[ic:ic + 4] in (b'HLPR', b'AOBJ', b'XALO'):
                    break
                ic += 1
            parse_chunk_block(buf, ic, nxt, anim, is_helper)
        elif sig == b'HLPR':
            parse_chunk_block(buf, pstart, nxt, anim, True)
        elif sig == b'AOBJ':
            anim.object_name, ic = _cstr(buf, pstart, nxt)
            while ic + 4 <= nxt:
                if buf[ic:ic + 4] in (b'XSEQ', b'SEQ0', b'AMSK'):
                    break
                ic += 1
            parse_chunk_block(buf, ic, nxt, anim, False)
        elif sig == b'MVEC':
            if pstart + 12 <= nxt:
                anim.movement_vector = struct.unpack_from('<3f', buf, pstart)
                parse_chunk_block(buf, pstart + 12, nxt, anim, is_helper)
        elif sig == b'TMEV':
            name, c2 = _cstr(buf, pstart, nxt)
            tm = struct.unpack_from('<f', buf, c2)[0] if c2 + 4 <= nxt else None
            anim.time_events.append((name, tm))
        elif sig == b'AMSK':
            anim.bone_mask = bytes(buf[pstart:nxt])
        elif sig == b'XALO':
            anim.xalo = bytes(buf[pstart:nxt])
        elif sig in (b'XSEQ', b'SEQ0'):
            tr = parse_xseq(buf, pstart, nxt)
            if tr is not None:
                tr.is_helper = is_helper
                (anim.helper_tracks if is_helper else anim.tracks).append(tr)
        cursor = nxt


def parse_anim(payload):
    buf = decompress_payload(payload)
    if buf is None:
        return None
    anim = Anim()
    anim.decomp_size = len(buf)
    c = 0
    if len(buf) >= 8 and struct.unpack_from('<I', buf, 0)[0] == UNCOMP_MAGIC:
        c = 4
        if buf[c:c + 4] == b'3DAF':
            c += 8
            while c < len(buf) and buf[c] != 0:
                c += 1
            c += 1
            c = (c + 3) & ~3
    parse_chunk_block(buf, c, len(buf), anim, False)
    return anim


# ---------------------------------------------------------------- validation

def validate_anim(anim, toc_duration=None):
    """Numeric-coherence checks. Returns dict of counters."""
    v = dict(tracks=0, quat_keys=0, quat_bad=0, pos_keys=0, pos_far=0,
             pal_oob=0, frame_mismatch=0, tail_bytes=0)
    for t in anim.tracks + anim.helper_tracks:
        v['tracks'] += 1
        for q in t.rot_pool:
            v['quat_keys'] += 1
            n = math.sqrt(sum(x * x for x in q))
            if not (0.99 <= n <= 1.01):
                v['quat_bad'] += 1
        for p in t.pos_pool:
            v['pos_keys'] += 1
            if max(abs(x) for x in p) > 100.0:
                v['pos_far'] += 1
        for i in t.rot_palette:
            if i >= max(1, len(t.rot_pool)):
                v['pal_oob'] += 1
        for i in t.pos_palette:
            if i >= max(1, len(t.pos_pool)):
                v['pal_oob'] += 1
        # palette length should match frameCount when palette present
        for pal in (t.rot_palette, t.pos_palette):
            if pal and abs(len(pal) - t.frame_count) > 1:
                v['frame_mismatch'] += 1
        v['tail_bytes'] += t.chunk_size - t.consumed
    return v


def track_summary(t):
    return ('  [%3d parent %3d] %-28s fps=%-5g frames=%-4d rot %3d keys/%3d pal  '
            'pos %3d keys/%3d pal  posF=%g' %
            (t.bone_index, t.parent_index, t.bone_name, t.fps, t.frame_count,
             len(t.rot_pool), len(t.rot_palette),
             len(t.pos_pool), len(t.pos_palette), t.position_factor))


def anim_to_json(name, anim):
    def tr(t):
        return dict(boneIndex=t.bone_index, parentIndex=t.parent_index,
                    boneName=t.bone_name, fps=t.fps, frameCount=t.frame_count,
                    preFPSFlag=t.pre_fps_flag, postFrameFlags=list(t.post_frame_flags),
                    positionFactor=t.position_factor, scalingFactor=t.scaling_factor,
                    rotKeys=[list(q) for q in t.rot_pool], rotPalette=t.rot_palette,
                    posKeys=[list(p) for p in t.pos_pool], posPalette=t.pos_palette)
    return dict(name=name, duration=anim.duration, isCyclic=anim.is_cyclic,
                objectName=anim.object_name,
                movementVector=list(anim.movement_vector) if anim.movement_vector else None,
                timeEvents=[dict(name=n, time=t) for n, t in anim.time_events],
                tracks=[tr(t) for t in anim.tracks],
                helperTracks=[tr(t) for t in anim.helper_tracks])


# ---------------------------------------------------------------- CLI

def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 1
    path = sys.argv[1]
    args = sys.argv[2:]
    b, entries = load_anim_entries(path)
    print('%d ANIM entries (types 6/7/9)' % len(entries))

    if '--list' in args:
        i = args.index('--list')
        pat = args[i + 1].upper() if i + 1 < len(args) else None
        for e in entries:
            if pat and pat not in e['name'].upper():
                continue
            print('%-60s type=%d size=%d' % (e['name'], e['type'], e['size']))
        return 0

    if '--entry' in args:
        name = args[args.index('--entry') + 1]
        matches = [e for e in entries if e['name'] == name] or \
                  [e for e in entries if name.upper() in e['name'].upper()]
        if not matches:
            print('no entry matching %r' % name)
            return 1
        e = matches[0]
        payload = b[e['offset']:e['offset'] + e['size']]
        anim = parse_anim(payload)
        print('%s  (type %d, %d bytes on disk -> %d decompressed)' %
              (e['name'], e['type'], e['size'], anim.decomp_size))
        print('rig=%r duration=%.4fs cyclic=%s mvec=%s events=%d' %
              (anim.object_name, anim.duration, anim.is_cyclic,
               anim.movement_vector, len(anim.time_events)))
        print('chunks: %s' % anim.chunk_counts)
        print('%d bone tracks, %d helper tracks' %
              (len(anim.tracks), len(anim.helper_tracks)))
        for t in anim.tracks:
            print(track_summary(t))
        for t in anim.helper_tracks:
            print('H' + track_summary(t)[1:])
        for n, tm in anim.time_events:
            print('  event %-30s @ %.3fs' % (n, tm))
        print('validate: %s' % validate_anim(anim))
        if '--json' in args:
            out = args[args.index('--json') + 1]
            with open(out, 'w') as f:
                json.dump(anim_to_json(e['name'], anim), f, indent=1)
            print('wrote %s' % out)
        return 0

    if '--validate' in args:
        i = args.index('--validate')
        n = int(args[i + 1]) if i + 1 < len(args) and args[i + 1].isdigit() else len(entries)
        agg = {}
        fails = []
        parsed = 0
        for e in entries[:n]:
            payload = b[e['offset']:e['offset'] + e['size']]
            try:
                anim = parse_anim(payload)
            except Exception as ex:
                fails.append((e['name'], str(ex)))
                continue
            if anim is None or (not anim.tracks and not anim.helper_tracks
                                and e['size'] > 64):
                fails.append((e['name'], '0 tracks (size %d)' % e['size']))
                continue
            parsed += 1
            toc_dur = struct.unpack_from('<f', e['info'])[0] if e['infosize'] >= 4 else None
            v = validate_anim(anim, toc_dur)
            for k, val in v.items():
                agg[k] = agg.get(k, 0) + val
        print('parsed OK: %d/%d   failures: %d' % (parsed, n, len(fails)))
        print('aggregate: %s' % agg)
        for name, msg in fails[:25]:
            print('  FAIL %-55s %s' % (name, msg))
        if len(fails) > 25:
            print('  ... %d more' % (len(fails) - 25))
        return 0

    print(__doc__)
    return 1


if __name__ == '__main__':
    sys.exit(main())
