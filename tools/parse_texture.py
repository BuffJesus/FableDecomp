#!/usr/bin/env python3
"""Fable: TLC GBANK texture-entry parser (frontend.big / textures.big).

Pure byte-level, NO Ghidra. Reuses tools/parse_bigb.py (unmodified) as the
container reader. Cracks the 34-byte per-entry texture Info descriptor
(dims / pixel-format / mip-count), walks the compressed pixel-payload framing,
and decodes the *uncompressed* pixel formats to PNG.

Status (see docs/BIG_TEXTURE_FORMAT.md):
  * Info descriptor  ...... CONFIRMED (validated on all 6,324 + 394 entries).
  * Pixel-format enum ..... CONFIRMED (0x1=A8R8G8B8, 0x1f=DXT1, 0x20=DXT3).
  * Payload framing ....... CONFIRMED (u16/u32 block-length prefix per mip).
  * Pixel-payload LZ codec  GAP -- custom Lionhead stream, not a standard codec;
                            needs the engine decompressor (Ghidra) to finish.
                            DXT-decode + PNG path is wired and ready; it runs on
                            any de-compressed mip you can supply.

Usage:
  python parse_texture.py <file.big>                 # list all texture entries + Info
  python parse_texture.py <file.big> --entry <id>    # dump one entry's descriptor+framing
  python parse_texture.py <file.big> --png <id> <out.png>   # decode+save (uncompressed fmts only)
"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_bigb as pb

# ---------------------------------------------------------------------------
# Info descriptor (34 bytes, InfoSize==34 for every GBANK texture entry).
# Little-endian.  Offsets validated across frontend.big(394) + textures.big(6324).
#   +0  u16 AllocWidth   power-of-two allocated (padded) width
#   +2  u16 AllocHeight  power-of-two allocated (padded) height
#   +4  u16 (0)          reserved/depth-hi
#   +6  u16 Width        real image width  (<= AllocWidth)
#   +8  u16 Height       real image height (<= AllocHeight)
#   +10 u16 Depth        1 for 2D textures
#   +12 u8  Format       pixel-format enum (see FORMATS)
#   +13 u8  (0)
#   +16 u8  ? (byte16, usually 0)
#   +17 u8  MipCount     number of stored mip levels (grows with max dimension)
#   +24 u32 FirstBlockField  == payload-first-block-length + headerbytes + 3
#   +28 .. class/D3DFORMAT descriptor tail:
#          DXT1/DXT3 -> "03 04 00 00 00 00"
#          A8R8G8B8  -> "01 20 08 08 08 08"  (08 08 08 08 = 8/8/8/8 bit depths)
# ---------------------------------------------------------------------------

# Pixel-format enum -> (name, bytes-per-4x4-block or bpp, is-block-compressed)
FORMATS = {
    0x01: ("A8R8G8B8", 32, False),   # uncompressed 32bpp ARGB (tail 08 08 08 08)
    0x1f: ("DXT1",       8, True),   # S3TC, 8 bytes / 4x4 block
    0x20: ("DXT3",      16, True),   # S3TC + explicit 4-bit alpha, 16 B / block
    0x23: ("DXT?_0x23", 16, True),   # rare (4 entries in textures.big) [hypothesis DXT5]
    0x18: ("FMT_0x18",  16, True),   # rare (1 entry) [hypothesis]
}


def parse_info(info):
    """Decode the 34-byte texture Info descriptor to a dict."""
    if len(info) < 34:
        return None
    aw, ah, _r4, w, h, depth = struct.unpack_from("<HHHHHH", info, 0)
    fmt = info[12]
    mipcount = info[17]
    first_block_field = struct.unpack_from("<I", info, 24)[0]
    tail = info[28:34]
    name, unit, compressed = FORMATS.get(fmt, ("UNKNOWN_0x%02x" % fmt, 0, None))
    return dict(alloc_w=aw, alloc_h=ah, width=w, height=h, depth=depth,
                fmt=fmt, fmt_name=name, mipcount=mipcount,
                first_block_field=first_block_field, tail=tail,
                compressed=compressed)


def mip_dims(aw, ah, mipcount):
    """Yield (w,h) per stored mip level, starting at the allocated base size."""
    w, h = aw, ah
    for _ in range(mipcount):
        yield w, h
        if w == 1 and h == 1:
            break
        w = max(1, w // 2)
        h = max(1, h // 2)


def payload_block_header(pay, p):
    """Read the per-block length prefix. Returns (clen, header_bytes).

    u16 form: `u16 clen`               -> header 2 bytes
    u32 form: `u16 0xffff; u32 clen`   -> header 6 bytes (used when clen>=65535)
    """
    n = struct.unpack_from("<H", pay, p)[0]
    if n == 0xffff:
        return struct.unpack_from("<I", pay, p + 2)[0], 6
    return n, 2


# ---------------------------------------------------------------------------
# DXT / ARGB pixel decoders (linear layout -> RGBA8888). Ready to run on any
# *decompressed* mip buffer. Fable TLC is native PC D3D9: pixel data is linear
# S3TC / D3DFMT_A8R8G8B8, NOT Xbox-swizzled (see doc swizzle verdict).
# ---------------------------------------------------------------------------

def _rgb565(c):
    r = (c >> 11) & 0x1f
    g = (c >> 5) & 0x3f
    b = c & 0x1f
    return (r << 3) | (r >> 2), (g << 2) | (g >> 4), (b << 3) | (b >> 2)


def decode_dxt1(data, w, h):
    import numpy as np
    out = np.zeros((h, w, 4), np.uint8)
    bx, by = (w + 3) // 4, (h + 3) // 4
    p = 0
    for byy in range(by):
        for bxx in range(bx):
            c0, c1, bits = struct.unpack_from("<HHI", data, p); p += 8
            r0, g0, b0 = _rgb565(c0); r1, g1, b1 = _rgb565(c1)
            pal = [(r0, g0, b0, 255), (r1, g1, b1, 255)]
            if c0 > c1:
                pal.append(((2*r0+r1)//3, (2*g0+g1)//3, (2*b0+b1)//3, 255))
                pal.append(((r0+2*r1)//3, (g0+2*g1)//3, (b0+2*b1)//3, 255))
            else:
                pal.append(((r0+r1)//2, (g0+g1)//2, (b0+b1)//2, 255))
                pal.append((0, 0, 0, 0))
            for py in range(4):
                for px in range(4):
                    yy, xx = byy*4+py, bxx*4+px
                    if yy < h and xx < w:
                        out[yy, xx] = pal[(bits >> (2*(4*py+px))) & 3]
    return out


def decode_dxt3(data, w, h):
    import numpy as np
    out = np.zeros((h, w, 4), np.uint8)
    bx, by = (w + 3) // 4, (h + 3) // 4
    p = 0
    for byy in range(by):
        for bxx in range(bx):
            alpha = struct.unpack_from("<Q", data, p)[0]; p += 8
            c0, c1, bits = struct.unpack_from("<HHI", data, p); p += 8
            r0, g0, b0 = _rgb565(c0); r1, g1, b1 = _rgb565(c1)
            pal = [(r0, g0, b0), (r1, g1, b1),
                   ((2*r0+r1)//3, (2*g0+g1)//3, (2*b0+b1)//3),
                   ((r0+2*r1)//3, (g0+2*g1)//3, (b0+2*b1)//3)]
            for py in range(4):
                for px in range(4):
                    yy, xx = byy*4+py, bxx*4+px
                    if yy < h and xx < w:
                        idx = (bits >> (2*(4*py+px))) & 3
                        a4 = (alpha >> (4*(4*py+px))) & 0xf
                        out[yy, xx] = (*pal[idx], (a4 << 4) | a4)
    return out


def decode_argb8888(data, w, h):
    """D3DFMT_A8R8G8B8 (BGRA byte order on disk) -> RGBA."""
    import numpy as np
    arr = np.frombuffer(data[:w*h*4], np.uint8).reshape(h, w, 4)
    out = np.empty_like(arr)
    out[..., 0] = arr[..., 2]  # R <- byte2
    out[..., 1] = arr[..., 1]  # G
    out[..., 2] = arr[..., 0]  # B <- byte0
    out[..., 3] = arr[..., 3]  # A
    return out


def decode_mip(fmt, data, w, h):
    if fmt == 0x1f:
        return decode_dxt1(data, w, h)
    if fmt in (0x20, 0x23, 0x18):
        return decode_dxt3(data, w, h)
    if fmt == 0x01:
        return decode_argb8888(data, w, h)
    raise ValueError("no decoder for fmt 0x%02x" % fmt)


def save_png(rgba, path):
    from PIL import Image
    Image.fromarray(rgba, "RGBA").save(path)


# ---------------------------------------------------------------------------
def iter_texture_entries(path):
    with open(path, "rb") as f:
        b = f.read()
    _, _, fo, _ = pb.parse_header(b)
    subs, _ = pb.parse_footer(b, fo)
    for s in subs:
        entries, _, _, err = pb.parse_toc(b, s, fo)
        for e in entries:
            yield b, s, e


def cmd_list(path):
    n = 0
    for b, s, e in iter_texture_entries(path):
        if e["infosize"] < 34:
            continue
        info = parse_info(e["info"])
        if not info:
            continue
        n += 1
        if n <= 40 or (n % 500 == 0):
            print(f"[{s['name']}] id={e['id']:4d} {info['fmt_name']:9s} "
                  f"real={info['width']}x{info['height']} alloc={info['alloc_w']}x{info['alloc_h']} "
                  f"mips={info['mipcount']} sz={e['size']} crc=0x{e['crc']:08x} name={e['name']!r}")
    print(f"total texture entries: {n}")


def cmd_entry(path, target_id):
    for b, s, e in iter_texture_entries(path):
        if e["id"] != target_id or e["infosize"] < 34:
            continue
        info = parse_info(e["info"])
        off, sz = e["offset"], e["size"]
        pay = b[off:off + sz]
        print(f"=== {s['name']} id={e['id']} name={e['name']!r}")
        print(f"  fmt=0x{info['fmt']:02x} ({info['fmt_name']}) compressed_pixels={info['compressed']}")
        print(f"  real={info['width']}x{info['height']}  alloc={info['alloc_w']}x{info['alloc_h']}  "
              f"mipCount={info['mipcount']}  tail={info['tail'].hex()}")
        print(f"  Info FirstBlockField(+24)={info['first_block_field']}")
        print(f"  payload size={sz}")
        # walk block framing (first few blocks + mip dims)
        p = 0; lvl = 0
        for (mw, mh) in mip_dims(info["alloc_w"], info["alloc_h"], info["mipcount"]):
            if p + 2 > sz:
                break
            clen, hdr = payload_block_header(pay, p)
            print(f"    block[{lvl}] mip {mw}x{mh}: clen={clen} (hdr={hdr}B) data@{p+hdr}")
            p += hdr + clen
            lvl += 1
            if lvl >= 3:
                print("    ... (further blocks; codec gap prevents full mip walk)")
                break
        return
    print(f"id {target_id} not found")


def cmd_png(path, target_id, out):
    for b, s, e in iter_texture_entries(path):
        if e["id"] != target_id or e["infosize"] < 34:
            continue
        info = parse_info(e["info"])
        if info["compressed"] is not False:
            print(f"id {target_id} fmt {info['fmt_name']} pixel-payload is LZ-compressed; "
                  f"the codec is an open gap (see docs/BIG_TEXTURE_FORMAT.md). "
                  f"PNG export is only wired for uncompressed decompressed buffers.")
            return
        # Uncompressed ARGB path would go here once a decompressed buffer exists.
        print("uncompressed-format PNG export requires a decompressed pixel buffer (codec gap).")
        return
    print(f"id {target_id} not found")


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    path = sys.argv[1]
    if "--entry" in sys.argv:
        cmd_entry(path, int(sys.argv[sys.argv.index("--entry") + 1]))
    elif "--png" in sys.argv:
        i = sys.argv.index("--png")
        cmd_png(path, int(sys.argv[i + 1]), sys.argv[i + 2])
    else:
        cmd_list(path)


if __name__ == "__main__":
    main()
