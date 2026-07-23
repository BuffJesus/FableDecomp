#!/usr/bin/env python3
r"""LZO1X-1 COMPRESSOR for Fable: TLC (pairs with lionhead_lz.decompress).

Port of the canonical LZO1X-1 encoder (EgoCore minilzo.c `lzo1x_1_compress` /
`do_compress`, MIT). LZO does not require byte-identical output to the reference
encoder — only a VALID token stream the decompressor accepts — so the correctness
gate is `lionhead_lz.decompress(compress(x)) == x` (our decompressor is a faithful
port of the engine's `lzo1x_decompress @ 0x00c06b90`). Cross-checked against
EgoCore minilzo and SilverChest `Lzo1x.cs`.

Token grammar emitted (matches lionhead_lz.decompress):
  * literal-run length token (t<0x10 at loop top; 0-extension when 0)
  * M2  match: t>=0x40, len 4..8, 11-bit offset (<=2048)      [2 bytes]
  * M3  match: t 0x20..0x3f, len extendable, 14-bit offset     [3+ bytes]
  * M4  match: t 0x10..0x1f, len extendable, offset >0x4000    [3+ bytes]
  * trailing-literal count folded into the match's op[-2] low 2 bits
  * EOF marker: 0x11 0x00 0x00

Public API:
  compress(src) -> bytes                       # raw LZO1X stream
  compress_fable_block(src) -> bytes           # Fable chunk framing (see below)
"""
import struct

M2_MAX_LEN = 8
M2_MAX_OFFSET = 0x0800
M3_MAX_LEN = 33
M3_MAX_OFFSET = 0x4000
M4_MAX_LEN = 9
M4_MAX_OFFSET = 0xBFFF
M3_MARKER = 0x20
M4_MARKER = 0x10
D_BITS = 14
D_SIZE = 1 << D_BITS
D_MASK = D_SIZE - 1


def _hash4(src, p):
    dv = src[p] | (src[p + 1] << 8) | (src[p + 2] << 16) | (src[p + 3] << 24)
    return ((dv * 0x1824429D) >> (32 - D_BITS)) & D_MASK


def _emit_literals(out, src, ii, t, first):
    """Emit a literal run of `t` bytes starting at ii. `first` = output empty."""
    if t == 0:
        return
    if first and t <= 238:
        out.append(17 + t)                     # initial-run marker (>0x11)
    elif t <= 3:
        out[-2] |= t                           # fold into previous match's op[-2]
    elif t <= 18:
        out.append(t - 3)
    else:
        tt = t - 18
        out.append(0)
        while tt > 255:
            tt -= 255
            out.append(0)
        out.append(tt)
    out += src[ii:ii + t]


def _emit_match(out, m_len, m_off):
    if m_len <= M2_MAX_LEN and m_off <= M2_MAX_OFFSET:
        m_off -= 1
        out.append(((m_len - 1) << 5) | ((m_off & 7) << 2))
        out.append((m_off >> 3) & 0xFF)
    elif m_off <= M3_MAX_OFFSET:
        m_off -= 1
        if m_len <= M3_MAX_LEN:
            out.append(M3_MARKER | (m_len - 2))
        else:
            m_len -= M3_MAX_LEN
            out.append(M3_MARKER)
            while m_len > 255:
                m_len -= 255
                out.append(0)
            out.append(m_len)
        out.append((m_off << 2) & 0xFF)
        out.append((m_off >> 6) & 0xFF)
    else:
        m_off -= 0x4000
        if m_len <= M4_MAX_LEN:
            out.append(M4_MARKER | ((m_off >> 11) & 8) | (m_len - 2))
        else:
            m_len -= M4_MAX_LEN
            out.append(M4_MARKER | ((m_off >> 11) & 8))
            while m_len > 255:
                m_len -= 255
                out.append(0)
            out.append(m_len)
        out.append((m_off << 2) & 0xFF)
        out.append((m_off >> 6) & 0xFF)     # high bit carried in the M4 marker


def compress(src):
    """Compress `src` to a raw LZO1X-1 stream (decodable by lionhead_lz.decompress)."""
    src = bytes(src)
    n = len(src)
    out = bytearray()
    if n <= 20:                                # too small for the match finder
        _emit_literals(out, src, 0, n, first=True)
        out += b'\x11\x00\x00'
        return bytes(out)

    d = [-1] * D_SIZE
    ip_end = n - 20
    ii = 0                                     # start of pending literals
    ip = 4                                     # skip first 4 (ti=0 path)
    while ip < ip_end:
        idx = _hash4(src, ip)
        m_pos = d[idx]
        d[idx] = ip
        if (m_pos < 0 or (ip - m_pos) > M4_MAX_OFFSET
                or src[m_pos:m_pos + 4] != src[ip:ip + 4]):
            ip += 1 + ((ip - ii) >> 5)         # literal skip step
            continue
        m_off = ip - m_pos
        # extend match
        m_len = 4
        maxlen = n - ip
        while m_len < maxlen and src[ip + m_len] == src[m_pos + m_len]:
            m_len += 1
        _emit_literals(out, src, ii, ip - ii, first=(len(out) == 0))
        _emit_match(out, m_len, m_off)
        ip += m_len
        ii = ip
    _emit_literals(out, src, ii, n - ii, first=(len(out) == 0))
    out += b'\x11\x00\x00'                      # EOF marker
    return bytes(out)


def compress_fable_block(src):
    r"""Fable chunk framing (inverse of fable_core._lzo_fable / EgoCore DecompressLZO):
    LZO-compress the first len-3 bytes into one chunk, then append the 3 trailing
    plain bytes. Emits `[u16 clen][clen LZO bytes][3 raw bytes]` (u32 escape if
    clen>=0xffff). If the compressed form is not smaller than a stored chunk,
    emit a STORED chunk (clen prefix header form still, but marks it uncompressed
    per the engine's stored-block path) — here we always emit compressed since the
    engine accepts it; callers wanting stored can use the existing clen==0 path."""
    src = bytes(src)
    if len(src) <= 3:
        return src                             # nothing to frame
    body = src[:-3]
    tail = src[-3:]
    comp = compress(body)
    if len(comp) < 0xFFFF:
        hdr = struct.pack('<H', len(comp))
    else:
        hdr = struct.pack('<HI', 0xFFFF, len(comp))
    return hdr + comp + tail
