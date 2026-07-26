#!/usr/bin/env python3
r"""Fable: TLC native TEXTURE WRITER -- image -> textures.big/frontend.big entry.

Route B: pure Python (numpy + optional Pillow), no external exes. Pairs with the
proven decode path (tools/parse_texture.py decoders + tools/lionhead_lz.py) and
the proven compressor (tools/lionhead_lz_compress.py, engine-faithful LZO1X-1).

ON-DISK ENTRY LAYOUT (validated 2026-07-20 on 227/230 random retail entries,
see docs/TEXTURE_WRITER.md; answer key = EgoCore TextureParser.h/TextureBuilder.h):

  Info (34 bytes) = CGraphicHeader(28) + CPixelFormatInit(6):
    +0  u16 Width        allocated (pow2) surface width
    +2  u16 Height       allocated (pow2) surface height
    +4  u16 Depth        0 (2D)
    +6  u16 FrameWidth   real (authored) width
    +8  u16 FrameHeight  real (authored) height
    +10 u16 FrameCount   1 (multi-frame sprites exist; writer emits 1)
    +12 u32 PixelFormatIdx  0x1f=DXT1, 0x20=DXT3, 0x01=A8R8G8B8
    +16 u8  TransparencyType   DXT1:0 (2=punch-through variant) DXT3:1 ARGB:0/1
    +17 u8  MipmapLevels
    +18 u8  Flags        0
    +19 u8  Padding      0
    +20 u32 FrameDataSize  UNCOMPRESSED byte size of mip 0
    +24 u32 MipSize0       ON-DISK byte size of the mip-0 compressed region
                           (all chunk headers + chunk bytes + 3 raw tail bytes).
                           0 => the whole payload is stored RAW (no LZO).
    +28 CPixelFormatInit {Type,ColourDepth,R,G,B,A}:
        DXT1: 03 04 00 00 00 00   DXT3: 02 08 00 00 00 00   ARGB: 01 20 08 08 08 08

  Payload, per frame:
    mip 0   : Fable chunked-LZO region: run of [u16 clen][clen LZO1X bytes]
              (clen==0xFFFF -> u32 escape; clen==0 -> stored chunk) producing
              rawlen-3 bytes, then the final 3 bytes stored raw.  (Skipped --
              stored raw -- when MipSize0 == 0.)
    mips 1+ : STORED RAW, tightly concatenated (no headers, no compression).

CLI:
  python texture_build.py build     <image> <out_prefix> [--format dxt1|dxt3|argb8888]
                                    [--mips N] [--dims WxH]
  python texture_build.py replace   <src.big> <out.big> <entry_name> <image>
                                    [--sub SUBBANK] [--format ...] [--raw-mip0]
  python texture_build.py roundtrip <src.big> [--entry NAME] [--samples N]
  python texture_build.py decode    <src.big> <entry_name> <out.png> [--crop-real]
  python texture_build.py selftest
"""
import os
import sys
import struct

import numpy as np

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)
import parse_bigb                      # container reader (unmodified)
import parse_texture                   # Info parser + DXT/ARGB decoders (unmodified)
import lionhead_lz                     # engine-recovered LZO1X decompressor
import lionhead_lz_compress            # proven LZO1X-1 compressor (READ-ONLY dep)
import big_write                       # container rebuild (READ-ONLY dep)

FMT_ARGB = 0x01
FMT_DXT1 = 0x1F
FMT_DXT3 = 0x20

# fmt -> (name, CPixelFormatInit tail, default TransparencyType)
# Tails confirmed empirically on retail textures.big/frontend.big (230-entry
# sample, 100% consistent per format) AND matching EgoCore TextureBuilder
# ImportImage (DXT1 Type=3/Depth=4, DXT3 Type=2/Depth=8, ARGB Type=1/Depth=32).
FMT_META = {
    FMT_DXT1: ("DXT1",     bytes([0x03, 0x04, 0, 0, 0, 0]), 0),
    FMT_DXT3: ("DXT3",     bytes([0x02, 0x08, 0, 0, 0, 0]), 1),
    FMT_ARGB: ("A8R8G8B8", bytes([0x01, 0x20, 8, 8, 8, 8]), 1),
}
FMT_BY_NAME = {"dxt1": FMT_DXT1, "dxt3": FMT_DXT3, "argb8888": FMT_ARGB,
               "argb": FMT_ARGB, "a8r8g8b8": FMT_ARGB}


# ===========================================================================
# helpers
# ===========================================================================
def mip_raw_len(fmt, w, h):
    """Uncompressed byte length of one mip level (linear surface)."""
    if fmt == FMT_ARGB:
        return w * h * 4
    unit = 8 if fmt == FMT_DXT1 else 16
    return ((w + 3) // 4) * ((h + 3) // 4) * unit


def full_mip_count(w, h, fmt):
    """EgoCore TextureBuilder rule: chain down to 4x4 for DXT, 1x1 for ARGB."""
    min_dim = 1 if fmt == FMT_ARGB else 4
    mips = 1
    while w > min_dim or h > min_dim:
        mips += 1
        w = max(1, w >> 1)
        h = max(1, h >> 1)
    return mips


def psnr(a, b):
    a = np.asarray(a, np.float64)
    b = np.asarray(b, np.float64)
    mse = np.mean((a - b) ** 2)
    if mse == 0:
        return float("inf")
    return 10.0 * np.log10(255.0 * 255.0 / mse)


def _pow2_up(v):
    p = 1
    while p < v:
        p <<= 1
    return p


# ===========================================================================
# DXT block ENCODERS (numpy-vectorized; stb_dxt-style PCA endpoints + one
# least-squares refinement pass). Validated by decoding with the project's own
# parse_texture.decode_dxt1/decode_dxt3 and asserting high PSNR.
# ===========================================================================
def _to_blocks(rgba):
    """HxWx4 uint8 -> (N,16,4) float64 blocks in raster order, edge-replicated
    to a multiple of 4 (same rule as EgoCore CompressDXT: clamp to w-1/h-1)."""
    h, w = rgba.shape[:2]
    H = (h + 3) & ~3
    W = (w + 3) & ~3
    if H != h or W != w:
        pad = np.empty((H, W, 4), np.uint8)
        pad[:h, :w] = rgba
        pad[h:, :w] = rgba[h - 1:h, :]
        pad[:, w:] = pad[:, w - 1:w]
        rgba = pad
    by, bx = H // 4, W // 4
    blk = rgba.reshape(by, 4, bx, 4, 4).transpose(0, 2, 1, 3, 4)
    return blk.reshape(by * bx, 16, 4).astype(np.float64), bx, by


def _quant565(rgb):
    """(N,3) float -> (N,) uint16 RGB565 with rounding."""
    rgb = np.clip(rgb, 0.0, 255.0)
    r = np.rint(rgb[:, 0] * 31.0 / 255.0).astype(np.uint16)
    g = np.rint(rgb[:, 1] * 63.0 / 255.0).astype(np.uint16)
    b = np.rint(rgb[:, 2] * 31.0 / 255.0).astype(np.uint16)
    return (r << 11) | (g << 5) | b


def _dequant565(c):
    """(N,) uint16 -> (N,3) float, using the DECODER's expansion
    ((r<<3)|(r>>2) etc, identical to parse_texture._rgb565)."""
    r = (c >> 11) & 0x1F
    g = (c >> 5) & 0x3F
    b = c & 0x1F
    out = np.empty((len(c), 3), np.float64)
    out[:, 0] = (r << 3) | (r >> 2)
    out[:, 1] = (g << 2) | (g >> 4)
    out[:, 2] = (b << 3) | (b >> 2)
    return out


def _pick_indices(px, e0, e1):
    """px (N,16,3), endpoints (N,3) dequantized. Returns idx (N,16) in 0..3
    against the 4-colour DXT palette [e0, e1, 2/3e0+1/3e1, 1/3e0+2/3e1]."""
    pal = np.stack([e0, e1,
                    (2.0 * e0 + e1) / 3.0,
                    (e0 + 2.0 * e1) / 3.0], axis=1)          # (N,4,3)
    d = px[:, :, None, :] - pal[:, None, :, :]               # (N,16,4,3)
    dist = np.einsum("nikc,nikc->nik", d, d)                 # (N,16,4)
    return np.argmin(dist, axis=2)


def _refine_endpoints(px, idx):
    """Least-squares endpoint fit given indices (stb_dxt OptimizeColorsBlock
    analogue). weights: colour = w0*e0 + w1*e1, w0 = (idx->{1,0,2/3,1/3})."""
    w0_tab = np.array([1.0, 0.0, 2.0 / 3.0, 1.0 / 3.0])
    w0 = w0_tab[idx]                                          # (N,16)
    w1 = 1.0 - w0
    a = np.sum(w0 * w0, axis=1)
    b = np.sum(w0 * w1, axis=1)
    c = np.sum(w1 * w1, axis=1)
    det = a * c - b * b
    px0 = np.einsum("ni,nic->nc", w0, px)
    px1 = np.einsum("ni,nic->nc", w1, px)
    safe = np.abs(det) > 1e-8
    e0 = np.zeros((len(idx), 3))
    e1 = np.zeros((len(idx), 3))
    d = np.where(safe, det, 1.0)[:, None]
    e0[:] = (c[:, None] * px0 - b[:, None] * px1) / d
    e1[:] = (a[:, None] * px1 - b[:, None] * px0) / d
    return e0, e1, safe


def _encode_color_blocks(px):
    """px (N,16,3) float. Returns (c0,c1,bits) uint arrays with c0>c1 (4-colour
    opaque mode) for every block."""
    n = len(px)
    mean = px.mean(axis=1, keepdims=True)
    cen = px - mean
    # principal axis by power iteration on the 3x3 covariance
    cov = np.einsum("nic,nid->ncd", cen, cen)
    v = np.ones((n, 3))
    for _ in range(6):
        v = np.einsum("ncd,nd->nc", cov, v)
        norm = np.linalg.norm(v, axis=1, keepdims=True)
        v = v / np.where(norm < 1e-12, 1.0, norm)
    proj = np.einsum("nic,nc->ni", cen, v)
    e0 = px[np.arange(n), np.argmax(proj, axis=1)]   # colour at max projection
    e1 = px[np.arange(n), np.argmin(proj, axis=1)]

    best = None
    for it in range(3):                              # initial + 2 refinements
        c0 = _quant565(e0)
        c1 = _quant565(e1)
        # force 4-colour (opaque) mode: c0 > c1
        swap = c0 < c1
        c0s = np.where(swap, c1, c0)
        c1s = np.where(swap, c0, c1)
        eq = c0s == c1s
        # degenerate flat block: nudge c1 down one blue step when possible
        c1s = np.where(eq & (c1s > 0), c1s - 1, c1s)
        c0s = np.where(eq & (c0s == 0), 1, c0s)
        d0 = _dequant565(c0s)
        d1 = _dequant565(c1s)
        idx = _pick_indices(px, d0, d1)
        pal = np.stack([d0, d1, (2 * d0 + d1) / 3, (d0 + 2 * d1) / 3], axis=1)
        err = np.einsum("nic->n", (px - pal[np.arange(n)[:, None], idx]) ** 2)
        if best is None:
            best = [c0s.copy(), c1s.copy(), idx.copy(), err.copy()]
        else:
            better = err < best[3]
            for slot, new in zip(best, (c0s, c1s, idx, err)):
                slot[better] = new[better]
        if it < 2:
            ne0, ne1, safe = _refine_endpoints(px, best[2])
            e0 = np.where(safe[:, None], ne0, _dequant565(best[0]))
            e1 = np.where(safe[:, None], ne1, _dequant565(best[1]))
    c0, c1, idx, _ = best
    sh = (2 * np.arange(16, dtype=np.uint64))[None, :]
    bits = np.sum(idx.astype(np.uint64) << sh, axis=1, dtype=np.uint64)
    return c0.astype(np.uint16), c1.astype(np.uint16), bits.astype(np.uint32)


def encode_dxt1(rgba):
    """HxWx4 uint8 -> DXT1 bytes (opaque 4-colour mode; 1-bit alpha NOT
    emitted -- transparent DXT1 is a documented gap)."""
    px, bx, by = _to_blocks(rgba)
    c0, c1, bits = _encode_color_blocks(px[:, :, :3])
    out = np.zeros((len(c0), 2), np.uint32)
    out[:, 0] = c0.astype(np.uint32) | (c1.astype(np.uint32) << 16)
    out[:, 1] = bits
    return out.astype("<u4").tobytes()


def encode_dxt3(rgba):
    """HxWx4 uint8 -> DXT3 bytes (explicit 4-bit alpha, rounded)."""
    px, bx, by = _to_blocks(rgba)
    c0, c1, bits = _encode_color_blocks(px[:, :, :3])
    # 4-bit alpha, rounded (EgoCore truncates with >>4; rounding decodes closer)
    a4 = np.minimum(15, ((px[:, :, 3].astype(np.uint64) + 8) >> 4))
    sh = (4 * np.arange(16, dtype=np.uint64))[None, :]
    aword = np.sum(a4 << sh, axis=1, dtype=np.uint64)
    n = len(c0)
    out = np.zeros((n, 4), np.uint32)
    out[:, 0] = (aword & 0xFFFFFFFF).astype(np.uint32)
    out[:, 1] = (aword >> 32).astype(np.uint32)
    out[:, 2] = c0.astype(np.uint32) | (c1.astype(np.uint32) << 16)
    out[:, 3] = bits
    return out.astype("<u4").tobytes()


def encode_argb(rgba):
    """HxWx4 uint8 RGBA -> on-disk A8R8G8B8 (BGRA byte order)."""
    out = np.empty_like(rgba)
    out[..., 0] = rgba[..., 2]
    out[..., 1] = rgba[..., 1]
    out[..., 2] = rgba[..., 0]
    out[..., 3] = rgba[..., 3]
    return out.tobytes()


def encode_mip(fmt, rgba):
    if fmt == FMT_DXT1:
        return encode_dxt1(rgba)
    if fmt == FMT_DXT3:
        return encode_dxt3(rgba)
    if fmt == FMT_ARGB:
        return encode_argb(rgba)
    raise ValueError("no encoder for fmt 0x%02x" % fmt)


def downsample(rgba):
    """2x2 box filter (EgoCore uses stbir bilinear; box is equivalent for the
    exact 1/2 case). Odd dims fall back to nearest of the clamped grid."""
    h, w = rgba.shape[:2]
    nh, nw = max(1, h >> 1), max(1, w >> 1)
    if h >= 2 and w >= 2 and h % 2 == 0 and w % 2 == 0:
        a = rgba.astype(np.uint16).reshape(nh, 2, nw, 2, 4)
        return ((a.sum(axis=(1, 3)) + 2) >> 2).astype(np.uint8)
    ys = np.minimum((np.arange(nh) * 2), h - 1)
    xs = np.minimum((np.arange(nw) * 2), w - 1)
    return rgba[ys][:, xs]


# ===========================================================================
# entry BUILD
# ===========================================================================
def build_info(fmt, alloc_w, alloc_h, real_w, real_h, mips, frame_data_size,
               mip_size0, transparency=None, flags=0, frame_count=1, depth=0):
    name, tail, def_transp = FMT_META[fmt]
    t = def_transp if transparency is None else transparency
    info = struct.pack("<HHHHHH", alloc_w, alloc_h, depth, real_w, real_h,
                       frame_count)
    info += struct.pack("<I", fmt)
    info += struct.pack("<4B", t, mips, flags, 0)
    info += struct.pack("<II", frame_data_size, mip_size0)
    info += tail
    assert len(info) == 34
    return info


def build_entry(rgba, fmt, mips=None, real_w=None, real_h=None,
                transparency=None, compress_mip0=True):
    """rgba: HxWx4 uint8, H/W MUST already be the allocated (pow2) dims.
    Returns (payload_bytes, info34_bytes)."""
    h, w = rgba.shape[:2]
    if mips is None:
        mips = full_mip_count(w, h, fmt)
    chain = [rgba]
    for _ in range(mips - 1):
        chain.append(downsample(chain[-1]))

    parts = []
    mip0_size_field = 0
    fds = 0
    for i, mip in enumerate(chain):
        raw = encode_mip(fmt, mip)
        if i == 0:
            fds = len(raw)
            if compress_mip0 and len(raw) > 3:
                blk = lionhead_lz_compress.compress_fable_block(raw)
                mip0_size_field = len(blk)
                parts.append(blk)
            else:
                mip0_size_field = 0     # engine rule: MipSize0==0 => stored raw
                parts.append(raw)
        else:
            parts.append(raw)           # mips 1+ are ALWAYS stored raw
    payload = b"".join(parts)
    info = build_info(fmt, w, h, real_w or w, real_h or h, len(chain), fds,
                      mip0_size_field, transparency=transparency)
    return payload, info


# ===========================================================================
# entry DECODE (the validation reader: full confirmed layout, all mips)
# ===========================================================================
def decode_entry(info_bytes, payload, max_mips=None):
    """Returns (info_dict, [RGBA ndarray per mip]) for frame 0."""
    aw, ah, depth, rw, rh, fc = struct.unpack_from("<HHHHHH", info_bytes, 0)
    fmt = struct.unpack_from("<I", info_bytes, 12)[0] & 0xFF
    transp, mips, flags, _pad = struct.unpack_from("<4B", info_bytes, 16)
    fds, mip0size = struct.unpack_from("<II", info_bytes, 20)
    mips = mips or 1
    out = []
    p = 0
    w, h = aw, ah
    for mi in range(mips):
        rawlen = mip_raw_len(fmt, w, h)
        if mi == 0 and mip0size > 0:
            raw, p2 = _read_chunked(payload, p, rawlen)
            if p2 - p != mip0size:
                sys.stderr.write("note: mip0 region %d != MipSize0 %d\n"
                                 % (p2 - p, mip0size))
            p = p2
        else:
            raw = bytes(payload[p:p + rawlen])
            p += rawlen
        if len(raw) < rawlen:
            raw += b"\x00" * (rawlen - len(raw))
        out.append(parse_texture.decode_mip(fmt, raw, w, h))
        w = max(1, w >> 1)
        h = max(1, h >> 1)
        if max_mips and len(out) >= max_mips:
            break
    d = dict(alloc_w=aw, alloc_h=ah, width=rw, height=rh, fmt=fmt,
             fmt_name=FMT_META.get(fmt, ("0x%02x" % fmt,))[0], mips=mips,
             frames=fc, transp=transp, flags=flags, fds=fds, mip0size=mip0size,
             consumed=p, payload_len=len(payload))
    return d, out


def _read_chunked(pay, p, rawlen):
    """Walk one Fable chunked-LZO region (rawlen-3 LZO'd + 3 raw tail bytes)."""
    target = rawlen - 3 if rawlen > 3 else 0
    out = bytearray()
    while len(out) < target:
        clen = struct.unpack_from("<H", pay, p)[0]
        p += 2
        if clen == 0xFFFF:
            clen = struct.unpack_from("<I", pay, p)[0]
            p += 4
        if clen == 0:                               # stored chunk
            take = target - len(out)
            out += pay[p:p + take]
            p += take
        else:
            out += lionhead_lz.decompress(bytes(pay[p:p + clen]))
            p += clen
    if rawlen >= 3:
        out += pay[p:p + 3]
        p += 3
    return bytes(out), p


# ===========================================================================
# container plumbing
# ===========================================================================
def load_big(path):
    with open(path, "rb") as f:
        buf = f.read()
    _, _, fo, _ = parse_bigb.parse_header(buf)
    subs, _ = parse_bigb.parse_footer(buf, fo)
    parsed = []
    for s in subs:
        entries, _, _, err = parse_bigb.parse_toc(buf, s, fo)
        if err:
            raise ValueError("TOC parse error in %s: %s" % (s["name"], err))
        parsed.append((s, entries))
    return buf, parsed


def find_entry(parsed, name, sub_filter=None):
    hits = []
    for s, entries in parsed:
        if sub_filter and sub_filter.upper() not in s["name"].upper():
            continue
        for e in entries:
            if e["name"] == name and e["infosize"] >= 34:
                hits.append((s, e))
    return hits


def info_abs_offset(buf, e):
    """Absolute offset of the InfoSize-byte Info blob inside the TOC record.
    Record: magic,id,type,size,offset,crc (24) + lpstr(name) + timestamp,
    depcount (8) + deps(lpstr each) + infosize(4) + info."""
    p = e["rec_off"] + 24
    nlen = struct.unpack_from("<I", buf, p)[0]
    p += 4 + nlen + 8
    for _ in range(e["depcount"]):
        dlen = struct.unpack_from("<I", buf, p)[0]
        p += 4 + dlen
    isz = struct.unpack_from("<I", buf, p)[0]
    assert isz == e["infosize"]
    p += 4
    assert buf[p:p + e["infosize"]] == e["info"]
    return p


def replace_entry(src_big, out_big, name, payload, info, sub_filter=None):
    """Patch entry `name`'s Info in the TOC and rebuild the container with the
    new payload. Writes out_big. NEVER call on the install file."""
    buf, parsed = load_big(src_big)
    hits = find_entry(parsed, name, sub_filter)
    if not hits:
        raise SystemExit("entry %r not found" % name)
    if len(hits) > 1 and not sub_filter:
        raise SystemExit("entry %r ambiguous across sub-banks %s; use --sub"
                         % (name, [s["name"] for s, _ in hits]))
    s, e = hits[0]
    mbuf = bytearray(buf)
    ioff = info_abs_offset(buf, e)
    if len(info) != e["infosize"]:
        raise SystemExit("info size mismatch (%d != %d)"
                         % (len(info), e["infosize"]))
    mbuf[ioff:ioff + len(info)] = info
    out = big_write.rebuild(bytes(mbuf), {(s["name"], e["name"]): payload})
    with open(out_big, "wb") as f:
        f.write(out)
    return s["name"], e


# ===========================================================================
# image IO
# ===========================================================================
def load_image_rgba(path):
    from PIL import Image
    img = Image.open(path).convert("RGBA")
    return np.asarray(img, np.uint8).copy()


def save_png(rgba, path):
    from PIL import Image
    Image.fromarray(rgba).save(path)


def fit_to_alloc(rgba, aw, ah):
    if rgba.shape[1] == aw and rgba.shape[0] == ah:
        return rgba
    from PIL import Image
    img = Image.fromarray(rgba, "RGBA").resize((aw, ah), Image.LANCZOS)
    return np.asarray(img, np.uint8).copy()


# ===========================================================================
# CLI commands
# ===========================================================================
def cmd_build(argv):
    img_path, out_prefix = argv[0], argv[1]
    fmt = FMT_BY_NAME[_opt(argv, "--format", "dxt1").lower()]
    mips = _opt(argv, "--mips", None)
    dims = _opt(argv, "--dims", None)
    rgba = load_image_rgba(img_path)
    real_h, real_w = rgba.shape[:2]
    if dims:
        aw, ah = (int(x) for x in dims.lower().split("x"))
    else:
        aw, ah = _pow2_up(real_w), _pow2_up(real_h)
    rgba = fit_to_alloc(rgba, aw, ah)
    payload, info = build_entry(rgba, fmt, mips=int(mips) if mips else None,
                                real_w=real_w, real_h=real_h)
    open(out_prefix + ".payload.bin", "wb").write(payload)
    open(out_prefix + ".info.bin", "wb").write(info)
    d, mips_dec = decode_entry(info, payload, max_mips=1)
    p = psnr(rgba, mips_dec[0])
    print("built %s %dx%d (real %dx%d) mips=%d payload=%d B info=34 B "
          "self-decode PSNR=%.2f dB" % (FMT_META[fmt][0], aw, ah, real_w,
                                        real_h, d["mips"], len(payload), p))


def cmd_replace(argv):
    src, out, name, img_path = argv[:4]
    sub = _opt(argv, "--sub", None)
    raw0 = "--raw-mip0" in argv
    buf, parsed = load_big(src)
    hits = find_entry(parsed, name, sub)
    if not hits:
        raise SystemExit("entry %r not found in %s" % (name, src))
    s, e = hits[0]
    old = parse_texture.parse_info(e["info"])
    fmt_opt = _opt(argv, "--format", None)
    fmt = FMT_BY_NAME[fmt_opt.lower()] if fmt_opt else old["fmt"]
    if fmt not in FMT_META:
        raise SystemExit("existing fmt 0x%02x unsupported; use --format" % fmt)
    old_transp = e["info"][16]
    rgba = load_image_rgba(img_path)
    rgba = fit_to_alloc(rgba, old["alloc_w"], old["alloc_h"])
    payload, info = build_entry(
        rgba, fmt, mips=old["mipcount"],
        real_w=old["width"], real_h=old["height"],
        transparency=old_transp if fmt == old["fmt"] else None,
        compress_mip0=not raw0)
    sub_name, _ = replace_entry(src, out, name, payload, info, sub)
    print("replaced [%s] %r in %s -> %s (%s %dx%d mips=%d payload %d -> %d B)"
          % (sub_name, name, src, out, FMT_META[fmt][0], old["alloc_w"],
             old["alloc_h"], old["mipcount"], e["size"], len(payload)))


def cmd_decode(argv):
    src, name, out_png = argv[:3]
    sub = _opt(argv, "--sub", None)
    crop_real = "--crop-real" in argv
    buf, parsed = load_big(src)
    hits = find_entry(parsed, name, sub)
    if not hits:
        raise SystemExit("entry %r not found" % name)
    s, e = hits[0]
    pay = buf[e["offset"]:e["offset"] + e["size"]]
    d, mips = decode_entry(e["info"], pay, max_mips=1)
    image = mips[0]
    if crop_real:
        image = image[:d["height"], :d["width"]]
    save_png(image, out_png)
    print("decoded [%s] %r %s %dx%d -> %s"
          % (s["name"], name, d["fmt_name"], image.shape[1], image.shape[0],
             out_png))


def cmd_roundtrip(argv):
    """Identity oracle: decode retail entry -> re-encode -> decode -> PSNR."""
    src = argv[0]
    target = _opt(argv, "--entry", None)
    n_samples = int(_opt(argv, "--samples", "8"))
    import random
    buf, parsed = load_big(src)
    cands = []
    for s, entries in parsed:
        for e in entries:
            if e["infosize"] < 34 or e["size"] == 0:
                continue
            fmt = e["info"][12]
            fc = struct.unpack_from("<H", e["info"], 10)[0]
            if fmt in FMT_META and fc <= 1:
                cands.append((s, e))
    if target:
        cands = [(s, e) for s, e in cands if e["name"] == target]
    else:
        random.seed(7)
        cands = random.sample(cands, min(n_samples, len(cands)))
    worst = float("inf")
    for s, e in cands:
        pay = buf[e["offset"]:e["offset"] + e["size"]]
        d, mips0 = decode_entry(e["info"], pay)
        fmt = d["fmt"]
        payload2, info2 = build_entry(mips0[0], fmt, mips=d["mips"],
                                      real_w=d["width"], real_h=d["height"],
                                      transparency=d["transp"])
        d2, mips1 = decode_entry(info2, payload2)
        ps = [psnr(a, b) for a, b in zip(mips0, mips1)]
        worst = min(worst, ps[0])
        ident = "IDENTICAL" if np.array_equal(mips0[0], mips1[0]) else ""
        print("%-34s %-8s %4dx%-4d mips=%d  base PSNR=%6.2f dB  "
              "(worst mip %.2f) %s" % (e["name"], d["fmt_name"], d["alloc_w"],
                                       d["alloc_h"], d["mips"], ps[0],
                                       min(ps), ident))
    print("worst base-mip PSNR over %d entries: %.2f dB" % (len(cands), worst))


def cmd_selftest(argv):
    """Synthetic pattern -> build -> decode -> PSNR, all three formats."""
    rgba = _test_pattern(256, 256)
    for fname, fmt in (("dxt1", FMT_DXT1), ("dxt3", FMT_DXT3),
                       ("argb8888", FMT_ARGB)):
        payload, info = build_entry(rgba, fmt)
        d, mips = decode_entry(info, payload)
        p = psnr(rgba, mips[0])
        assert d["consumed"] == len(payload), (d["consumed"], len(payload))
        status = "OK" if (p > 30 or fmt == FMT_ARGB) else "LOW"
        if fmt == FMT_ARGB:
            assert p == float("inf"), "ARGB must be lossless, got %.2f" % p
        print("selftest %-8s: mips=%d payload=%d B PSNR=%s dB [%s]"
              % (fname, d["mips"], len(payload),
                 "inf" if p == float("inf") else "%.2f" % p, status))
    print("selftest passed")


def _test_pattern(w, h):
    """Deterministic gradient + rings + text-like ticks, with alpha ramp."""
    y, x = np.mgrid[0:h, 0:w]
    rgba = np.zeros((h, w, 4), np.uint8)
    rgba[..., 0] = (x * 255 // max(1, w - 1)).astype(np.uint8)
    rgba[..., 1] = (y * 255 // max(1, h - 1)).astype(np.uint8)
    r = np.hypot(x - w / 2, y - h / 2)
    rgba[..., 2] = ((np.sin(r / 6.0) * 0.5 + 0.5) * 255).astype(np.uint8)
    rgba[..., 3] = 255
    rgba[(x // 16 + y // 16) % 2 == 0, 3] = 224
    return rgba


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
    if cmd == "build":
        cmd_build(argv)
    elif cmd == "replace":
        cmd_replace(argv)
    elif cmd == "decode":
        cmd_decode(argv)
    elif cmd == "roundtrip":
        cmd_roundtrip(argv)
    elif cmd == "selftest":
        cmd_selftest(argv)
    else:
        print(__doc__)


if __name__ == "__main__":
    main()
