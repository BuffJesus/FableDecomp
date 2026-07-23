#!/usr/bin/env python3
"""parse_lut.py - reader for Fable: TLC `.lut` audio banks.

Magic: `LiOnHeAdLHAudioBankCompData`. These are the voice-over/dialogue audio
banks that sit between text.big (text) and dialogue.big (lipsync); all three are
keyed by the same ordinal <N>. See docs/LUT_AUDIOBANK_FORMAT.md.

On-disk layout (byte-verified, pure Python, no Ghidra):

  0x00  char[28]  Magic = "LiOnHeAdLHAudioBankCompData\0"
  0x1C  u32[3]    reserved (0)
  0x28  u32       TocOffset  (file offset of the LHAudioBankLookupTable directory)
  --- then EntryCount clip records, back-to-back, starting at 0x2C ---
  each clip record:
    +0x00 u32     Index          (1-based ordinal <N>; joins text.big/dialogue.big)
    +0x04 u32     0x56220001     (const: 0x5622=22050 sample rate | 0x0001 channels)
    +0x08 u32     0x00019C40     (const 105536)
    +0x0C u32     0x01010000     (const flags)
    +0x10 u32     Flags2         (varies per clip, ~0x0064xxxx)
    +0x14 f32     MinDistance    (1.5 / 5.0)
    +0x18 f32     MaxDistance    (18.0 / 25.0)
    +0x1C u32     Priority/Range (500 / 1000)
    +0x20 u32     0xFFFFFFFF     (loop/terminator sentinel)
    +0x24 ...     RIFF/WAVE clip (standard RIFF; fmt tag 0x0069 = Xbox ADPCM)
  --- directory at TocOffset ---
    LHAudioBankLookupTable  (32-byte NUL-padded struct name)
      u32 StructSize(=0x14), u32 MaxEntries(=500/1000), u32 Count, u32 Count,
      u32 LookupDataOffset, u32 reserved
    LHFileSegmentBankInfo   (32-byte NUL-padded struct name) + fields
    comment "\\Generated bank for Fable"
    ... "NoFPNameYet" ...

Usage:
  python parse_lut.py <file.lut> [--dump N] [--extract N OUT.wav] [--list]
"""
import struct, sys, os

MAGIC = b"LiOnHeAdLHAudioBankCompData\x00"
CLIP_HDR = 36  # bytes of per-clip header preceding the RIFF

def parse_header(b):
    magic = b[0:28]
    toc_off = struct.unpack_from('<I', b, 0x28)[0]
    return magic, toc_off

def riff_size(b, o):
    """Return total RIFF span (8 + RIFFsize) if a valid RIFF/WAVE is at o, else None."""
    if b[o:o+4] != b'RIFF':
        return None
    rs = struct.unpack_from('<I', b, o+4)[0]
    if b[o+8:o+12] != b'WAVE':
        return None
    return 8 + rs

def parse_fmt(b, riff_off):
    """Parse the fmt chunk of the RIFF at riff_off. Returns dict or None."""
    p = riff_off + 12
    end = riff_off + 8 + struct.unpack_from('<I', b, riff_off+4)[0]
    out = {}
    while p + 8 <= end:
        cid = b[p:p+4]; csz = struct.unpack_from('<I', b, p+4)[0]
        if cid == b'fmt ':
            tag, ch, sr, abps, ba, bps = struct.unpack_from('<HHIIHH', b, p+8)
            out = dict(fmt_tag=tag, channels=ch, sample_rate=sr,
                       avg_bps=abps, block_align=ba, bits=bps,
                       fmt_chunk_size=csz)
            if csz >= 18:
                out['cb_extra'] = b[p+8+16:p+8+csz].hex()
        elif cid == b'data':
            out['data_size'] = csz; out['data_off'] = p+8
        p += 8 + csz + (csz & 1)
    return out

def walk(b):
    """Walk all clip records. Returns (clips, stop_pos, toc_off)."""
    magic, toc_off = parse_header(b)
    clips = []
    p = 0x2c  # first clip record begins right after TocOffset u32
    while p + CLIP_HDR < len(b):
        # clip header
        idx = struct.unpack_from('<I', b, p)[0]
        const1 = struct.unpack_from('<I', b, p+4)[0]
        # heuristic stop: RIFF must follow the 36-byte header
        riff_off = p + CLIP_HDR
        span = riff_size(b, riff_off)
        if span is None:
            break
        f10, mind, maxd, prio, sentinel = struct.unpack_from('<IffII', b, p+0x10)
        clips.append(dict(index=idx, rec_off=p, riff_off=riff_off,
                          riff_span=span, const1=const1, flags2=f10,
                          min_dist=mind, max_dist=maxd, prio=prio,
                          sentinel=sentinel))
        p = riff_off + span
    return clips, p, toc_off

def main():
    if len(sys.argv) < 2:
        print(__doc__); return
    path = sys.argv[1]
    b = open(path, 'rb').read()
    magic, toc_off = parse_header(b)
    print(f"=== {os.path.basename(path)} ({len(b):,} bytes) ===")
    print(f"  magic={magic!r}")
    print(f"  TocOffset=0x{toc_off:x} ({toc_off})  (file size 0x{len(b):x})")
    if magic != MAGIC:
        print("  !! magic mismatch"); return
    clips, stop, _ = walk(b)
    print(f"  clips walked: {len(clips):,}   stopped at 0x{stop:x}")
    print(f"  region after clips -> TocOffset gap: 0x{toc_off - stop:x}")
    # directory marker check
    dir_pos = b.find(b'LHAudioBankLookupTable', stop-4 if stop>4 else 0)
    print(f"  LHAudioBankLookupTable @ 0x{dir_pos:x}" if dir_pos>=0 else "  (no directory marker found)")
    # codec from first clip
    if clips:
        fmt = parse_fmt(b, clips[0]['riff_off'])
        tagname = {0x69: 'Xbox/IMA ADPCM (WAVE_FORMAT_XBOX_ADPCM)',
                   0x11: 'IMA ADPCM', 0x01: 'PCM', 0x161: 'WMA'}.get(fmt.get('fmt_tag'), 'unknown')
        print(f"  codec: fmt_tag=0x{fmt['fmt_tag']:x} = {tagname}; "
              f"{fmt['channels']}ch {fmt['sample_rate']}Hz {fmt['bits']}bit "
              f"blockAlign={fmt['block_align']}")
        idxs = [c['index'] for c in clips]
        print(f"  index range: {min(idxs)}..{max(idxs)}  "
              f"contiguous={idxs==list(range(idxs[0], idxs[0]+len(idxs)))}")
        # bounds check
        oob = [c for c in clips if c['riff_off']+c['riff_span'] > len(b)]
        print(f"  out-of-bounds clips: {len(oob)}")

    if '--list' in sys.argv:
        for c in clips[:50]:
            print(f"    N={c['index']:>6} rec@0x{c['rec_off']:x} riff@0x{c['riff_off']:x} "
                  f"span={c['riff_span']} minD={c['min_dist']} maxD={c['max_dist']} prio={c['prio']}")
    if '--dump' in sys.argv:
        n = int(sys.argv[sys.argv.index('--dump')+1])
        for c in clips[:n]:
            fmt = parse_fmt(b, c['riff_off'])
            print(f"    N={c['index']} riff@0x{c['riff_off']:x} span={c['riff_span']} "
                  f"data={fmt.get('data_size')}B fmt=0x{fmt['fmt_tag']:x} "
                  f"{fmt['sample_rate']}Hz flags2=0x{c['flags2']:x}")
    if '--extract' in sys.argv:
        i = sys.argv.index('--extract')
        want = int(sys.argv[i+1]); out = sys.argv[i+2]
        for c in clips:
            if c['index'] == want:
                open(out, 'wb').write(b[c['riff_off']:c['riff_off']+c['riff_span']])
                print(f"  wrote clip N={want} -> {out} ({c['riff_span']} bytes)")
                break
        else:
            print(f"  clip N={want} not found")

if __name__ == '__main__':
    main()
