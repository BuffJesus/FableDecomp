#!/usr/bin/env python3
"""Fable: TLC proprietary .big pixel-payload decompressor.

RECOVERED FROM THE ENGINE. The codec is **LZO1X** (LZO / miniLZO family). The
engine ships the reference decompressor `lzo1x_decompress` at retail address
`0x00c06b90` (and a bounds-checked `lzo1x_decompress_safe` at `0x00c08170`),
decompiled from Fable.exe; Ghidra BSim matched it at sim=1.0 to the LZO1X
reference in the ego_r (EgoCore/Fable2) signature DB. The grammar below is a
faithful port of that routine, validated by decoding real textures to coherent
images (see docs/LIONHEAD_LZ.md).

Per-mip block framing (see docs/BIG_TEXTURE_FORMAT.md):
    block := u16 clen                 ; header 2 bytes, when clen < 0xffff
           | u16 0xffff , u32 clen    ; header 6 bytes, when clen >= 0xffff
           followed by `clen` bytes of the LZO1X stream.
Decompressed output is raw linear S3TC (DXT1/DXT3) or A8R8G8B8 pixel data.

Public API:
    decompress(src, expected_len=None) -> bytes
"""


def decompress(src, expected_len=None):
    """LZO1X-1 decompression. `src` is exactly the compressed stream (clen bytes).

    Faithful port of engine `lzo1x_decompress` @ 0x00c06b90. LZO is
    self-terminating via the M4 end-of-stream marker (token 0x11 with a zero
    encoded distance), so `expected_len` is optional and used only for a size
    sanity check. Returns the decompressed bytes.

    Token grammar (t = the token byte):
      * First byte >= 0x12  -> initial literal run of (t - 0x11) bytes.
      * At loop top a token t < 0x10 is a LITERAL-RUN length token:
            length = (t + 3), with the standard zero-byte length extension
            (base 15) when t == 0.
      * Match tokens by range:
            t 0x40..0xff : M2  length 2..8, 11-bit distance   (1 extra byte)
            t 0x20..0x3f : M3  extendable length, 14-bit dist  (2 extra bytes)
            t 0x10..0x1f : M4  extendable length, 14-bit dist + high bit
                               (2 extra bytes); encoded distance 0 => EOS.
            t 0x00..0x0f : short match. R1 (length 3, distance base 0x0801) when
                           it immediately follows a literal run; otherwise M1
                           (length 2, distance base 1) in the trailing-literal
                           match chain.
      * TRAILING LITERALS after every match: count = (byte 2 positions back in
        the input) & 3, i.e. `src[ip-2] & 3` once all of the match's bytes are
        consumed. For M1/M2 that byte is the token; for M3/M4 it is the low
        distance byte -- this is the exact LZO1X rule and the one subtlety that
        must be gotten right. A nonzero count chains directly into another match
        (M1 semantics).
    """
    ip = 0
    op = bytearray()

    def emit_match(dist, length):
        m = len(op) - dist
        if m < 0:
            raise ValueError("lzo1x: bad match distance %d at op=%d, ip=%d"
                             % (dist, len(op), ip))
        for _ in range(length):
            op.append(op[m]); m += 1

    t = src[ip]            # PEEK the first byte (do NOT consume yet)
    from_lit = False       # True => a following t<0x10 short match is R1, not M1
    goto_match = False     # True => `t` already holds a match token to decode

    # ---- initial literal run (engine top: first byte >= 0x12) ----
    if t > 0x11:
        ip += 1            # consume the initial-run token
        t -= 0x11
        for _ in range(t):
            op.append(src[ip]); ip += 1
        from_lit = (t >= 4)   # t<4 -> M1 chain (LAB_c80); t>=4 -> R1 (LAB_c3a)
        t = src[ip]; ip += 1
        goto_match = True
    # else: first byte <= 0x11 -> leave ip at 0 so the main loop consumes it as
    # the FIRST token (a literal-run when < 0x10). Previously this byte was
    # consumed here and then re-read in the loop, SKIPPING it -> a 1-byte desync
    # on any stream starting <= 0x11 (e.g. a large initial literal run "00 ..").
    # Fixes MESH_SUMMONER_BLUE_01 / MESH_JOB_MASK_01 (and any such mesh).

    while True:
        if not goto_match:
            t = src[ip]; ip += 1
            if t < 0x10:
                # literal-run length token
                if t == 0:
                    while src[ip] == 0:
                        t += 255; ip += 1
                    t += 15 + src[ip]; ip += 1
                for _ in range(t + 3):
                    op.append(src[ip]); ip += 1
                t = src[ip]; ip += 1
                from_lit = True
        goto_match = False

        # ---- match decode ----
        if t >= 0x40:
            length = (t >> 5) + 1
            h = src[ip]; ip += 1
            dist = ((t >> 2) & 7) + (h << 3) + 1
        elif t >= 0x20:
            length = t & 0x1f
            if length == 0:
                while src[ip] == 0:
                    length += 255; ip += 1
                length += 0x1f + src[ip]; ip += 1
            length += 2
            lo = src[ip]; hi = src[ip + 1]; ip += 2
            dist = ((hi << 6) + (lo >> 2)) + 1
        elif t >= 0x10:
            length = t & 7
            if length == 0:
                while src[ip] == 0:
                    length += 255; ip += 1
                length += 7 + src[ip]; ip += 1
            length += 2
            lo = src[ip]; hi = src[ip + 1]; ip += 2
            dist = ((t & 8) << 11) + ((hi << 6) + (lo >> 2))
            if dist == 0:
                break  # end-of-stream marker
            dist += 0x4000
        else:
            h = src[ip]; ip += 1
            if from_lit:
                dist = (t >> 2) + (h << 2) + 0x801   # R1: length 3
                length = 3
            else:
                dist = (t >> 2) + (h << 2) + 1       # M1: length 2
                length = 2

        emit_match(dist, length)

        # ---- trailing literals: src[ip-2] & 3 ----
        tl = src[ip - 2] & 3
        from_lit = False
        if tl:
            for _ in range(tl):
                op.append(src[ip]); ip += 1
            t = src[ip]; ip += 1
            goto_match = True     # chain directly into the next (M1) match

    if expected_len is not None and len(op) != expected_len \
            and len(op) - expected_len != -3:
        # -3 is normal Fable chunked framing (the 3 trailing plain bytes are
        # appended by the CALLER, not the LZO stream) -- not worth a warning.
        import sys
        sys.stderr.write("lzo1x: NOTE output %d != expected %d (%+d)\n"
                         % (len(op), expected_len, len(op) - expected_len))
    return bytes(op)


if __name__ == "__main__":
    import sys
    data = open(sys.argv[1], "rb").read()
    exp = int(sys.argv[2]) if len(sys.argv) > 2 else None
    out = decompress(data, exp)
    sys.stderr.write("decompressed %d -> %d bytes (expected %s)\n"
                     % (len(data), len(out), exp))
    if len(sys.argv) > 3:
        open(sys.argv[3], "wb").write(out)
    else:
        sys.stdout.buffer.write(out)
