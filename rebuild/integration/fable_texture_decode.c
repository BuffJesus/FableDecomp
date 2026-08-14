/* fable_texture_decode.c — see fable_texture_decode.h.
 * Faithful native port of tools/lionhead_lz.py (LZO1X-1) + the Fable chunk
 * framing + tools/parse_texture.py DXT1/DXT3/ARGB decoders. */
#include "fable_texture_decode.h"
#include <string.h>
#include <stdlib.h>

/* ---- LZO1X-1 decompress (engine lzo1x_decompress @ 0x00c06b90) ---- */
/* Writes into dst (bounded by dstCap). Returns bytes written or negative. */
int FableLzo1xDecompress(const unsigned char* src, unsigned int srcLen,
                         unsigned char* dst, unsigned int dstCap)
{
    unsigned int ip = 0, op = 0;
    unsigned int t;
    int from_lit = 0;      /* a following t<0x10 short match is R1, not M1 */
    int goto_match = 0;    /* t already holds a match token to decode */

    #define NEED_IN(n)  do { if (ip + (n) > srcLen) return -1; } while (0)
    #define NEED_OUT(n) do { if (op + (n) > dstCap) return -2; } while (0)
    #define EMIT_MATCH(dist, length) do {                       \
            unsigned int _d = (dist), _l = (length), _k;        \
            if (_d == 0 || _d > op) return -3;                  \
            NEED_OUT(_l);                                        \
            for (_k = 0; _k < _l; ++_k) { dst[op] = dst[op - _d]; ++op; } \
        } while (0)

    if (srcLen == 0) return 0;
    t = src[ip];   /* PEEK first byte, do not consume */

    if (t > 0x11) {
        ++ip;                       /* consume initial-run token */
        t -= 0x11;
        NEED_IN(t); NEED_OUT(t);
        { unsigned int k; for (k = 0; k < t; ++k) dst[op++] = src[ip++]; }
        from_lit = (t >= 4);
        NEED_IN(1); t = src[ip++];
        goto_match = 1;
    }

    for (;;) {
        if (!goto_match) {
            NEED_IN(1); t = src[ip++];
            if (t < 0x10) {
                if (t == 0) {
                    NEED_IN(1);
                    while (src[ip] == 0) { t += 255; ++ip; NEED_IN(1); }
                    t += 15 + src[ip++];
                }
                { unsigned int run = t + 3, k;
                  NEED_IN(run); NEED_OUT(run);
                  for (k = 0; k < run; ++k) dst[op++] = src[ip++]; }
                NEED_IN(1); t = src[ip++];
                from_lit = 1;
            }
        }
        goto_match = 0;

        {
            unsigned int length, dist, lo, hi, h;
            if (t >= 0x40) {                     /* M2 */
                length = (t >> 5) + 1;
                NEED_IN(1); h = src[ip++];
                dist = ((t >> 2) & 7) + (h << 3) + 1;
            } else if (t >= 0x20) {              /* M3 */
                length = t & 0x1f;
                if (length == 0) {
                    NEED_IN(1);
                    while (src[ip] == 0) { length += 255; ++ip; NEED_IN(1); }
                    length += 0x1f + src[ip++];
                }
                length += 2;
                NEED_IN(2); lo = src[ip]; hi = src[ip + 1]; ip += 2;
                dist = ((hi << 6) + (lo >> 2)) + 1;
            } else if (t >= 0x10) {              /* M4 */
                length = t & 7;
                if (length == 0) {
                    NEED_IN(1);
                    while (src[ip] == 0) { length += 255; ++ip; NEED_IN(1); }
                    length += 7 + src[ip++];
                }
                length += 2;
                NEED_IN(2); lo = src[ip]; hi = src[ip + 1]; ip += 2;
                dist = ((t & 8) << 11) + ((hi << 6) + (lo >> 2));
                if (dist == 0) break;            /* end-of-stream */
                dist += 0x4000;
            } else {                             /* M1 / R1 short match */
                NEED_IN(1); h = src[ip++];
                if (from_lit) { dist = (t >> 2) + (h << 2) + 0x801; length = 3; }
                else          { dist = (t >> 2) + (h << 2) + 1;     length = 2; }
            }

            EMIT_MATCH(dist, length);

            /* trailing literals: src[ip-2] & 3 */
            { unsigned int tl = src[ip - 2] & 3;
              from_lit = 0;
              if (tl) {
                  unsigned int k;
                  NEED_IN(tl); NEED_OUT(tl);
                  for (k = 0; k < tl; ++k) dst[op++] = src[ip++];
                  NEED_IN(1); t = src[ip++];
                  goto_match = 1;
              }
            }
        }
    }
    return (int)op;
    #undef NEED_IN
    #undef NEED_OUT
    #undef EMIT_MATCH
}

/* ---- Fable chunked-LZO region: [u16 clen][clen bytes], 0xFFFF->u32 escape,
 * clen==0 -> stored; produces (rawlen-3) bytes + 3 raw tail bytes ---- */
static int read_chunked(const unsigned char* pay, unsigned int paylen,
                        unsigned int* pp, unsigned int rawlen,
                        unsigned char* out, unsigned int outCap)
{
    unsigned int p = *pp, produced = 0;
    unsigned int target = rawlen > 3 ? rawlen - 3 : 0;
    while (produced < target) {
        unsigned int clen;
        if (p + 2 > paylen) return -1;
        clen = pay[p] | (pay[p + 1] << 8); p += 2;
        if (clen == 0xFFFF) {
            if (p + 4 > paylen) return -1;
            clen = pay[p] | (pay[p + 1] << 8) | (pay[p + 2] << 16) | (pay[p + 3] << 24);
            p += 4;
        }
        if (clen == 0) {                         /* stored chunk */
            unsigned int take = target - produced;
            if (p + take > paylen || produced + take > outCap) return -2;
            memcpy(out + produced, pay + p, take);
            produced += take; p += take;
        } else {
            int n;
            if (p + clen > paylen) return -3;
            n = FableLzo1xDecompress(pay + p, clen, out + produced, outCap - produced);
            if (n < 0) return -10 + n;
            produced += (unsigned int)n; p += clen;
        }
    }
    if (rawlen >= 3) {                            /* 3 raw tail bytes */
        if (p + 3 > paylen || produced + 3 > outCap) return -4;
        memcpy(out + produced, pay + p, 3);
        produced += 3; p += 3;
    }
    *pp = p;
    return (int)produced;
}

/* ---- DXT decoders ---- */
static void rgb565(unsigned int c, int* r, int* g, int* b)
{
    int rr = (c >> 11) & 0x1f, gg = (c >> 5) & 0x3f, bb = c & 0x1f;
    *r = (rr << 3) | (rr >> 2);
    *g = (gg << 2) | (gg >> 4);
    *b = (bb << 3) | (bb >> 2);
}

/* Decode DXT1 (fmt 0x1f) or DXT3 (fmt 0x20) `raw` into RGBA (w*h*4). */
static void decode_dxt(const unsigned char* raw, unsigned int w, unsigned int h,
                       int dxt3, unsigned char* out)
{
    unsigned int bx = (w + 3) / 4, by = (h + 3) / 4, byy, bxx;
    unsigned int p = 0;
    for (byy = 0; byy < by; ++byy) {
        for (bxx = 0; bxx < bx; ++bxx) {
            unsigned long long alpha = 0;
            unsigned int c0, c1, bits, py, px;
            int r[4], g[4], b[4];
            if (dxt3) {
                unsigned int k;
                for (k = 0; k < 8; ++k) alpha |= (unsigned long long)raw[p + k] << (8 * k);
                p += 8;
            }
            c0 = raw[p] | (raw[p + 1] << 8);
            c1 = raw[p + 2] | (raw[p + 3] << 8);
            bits = raw[p + 4] | (raw[p + 5] << 8) | (raw[p + 6] << 16) | ((unsigned int)raw[p + 7] << 24);
            p += 8;
            rgb565(c0, &r[0], &g[0], &b[0]);
            rgb565(c1, &r[1], &g[1], &b[1]);
            if (dxt3 || c0 > c1) {               /* DXT3 always 4-colour */
                r[2] = (2 * r[0] + r[1]) / 3; g[2] = (2 * g[0] + g[1]) / 3; b[2] = (2 * b[0] + b[1]) / 3;
                r[3] = (r[0] + 2 * r[1]) / 3; g[3] = (g[0] + 2 * g[1]) / 3; b[3] = (b[0] + 2 * b[1]) / 3;
            } else {
                r[2] = (r[0] + r[1]) / 2; g[2] = (g[0] + g[1]) / 2; b[2] = (b[0] + b[1]) / 2;
                r[3] = g[3] = b[3] = 0;
            }
            for (py = 0; py < 4; ++py) {
                for (px = 0; px < 4; ++px) {
                    unsigned int yy = byy * 4 + py, xx = bxx * 4 + px;
                    unsigned int shift = 2 * (4 * py + px);
                    unsigned int idx = (bits >> shift) & 3;
                    int a;
                    if (yy >= h || xx >= w) continue;
                    if (dxt3) {
                        int a4 = (int)((alpha >> (4 * (4 * py + px))) & 0xf);
                        a = (a4 << 4) | a4;
                    } else {
                        a = (!(c0 > c1) && idx == 3) ? 0 : 255;
                    }
                    {
                        unsigned char* o = out + (yy * w + xx) * 4;
                        o[0] = (unsigned char)r[idx];
                        o[1] = (unsigned char)g[idx];
                        o[2] = (unsigned char)b[idx];
                        o[3] = (unsigned char)a;
                    }
                }
            }
        }
    }
}

static void decode_argb(const unsigned char* raw, unsigned int w, unsigned int h,
                        unsigned char* out)
{
    unsigned int i, n = w * h;
    for (i = 0; i < n; ++i) {
        out[i * 4 + 0] = raw[i * 4 + 2];   /* R <- byte2 (BGRA on disk) */
        out[i * 4 + 1] = raw[i * 4 + 1];   /* G */
        out[i * 4 + 2] = raw[i * 4 + 0];   /* B <- byte0 */
        out[i * 4 + 3] = raw[i * 4 + 3];   /* A */
    }
}

static unsigned int mip_raw_len(unsigned int fmt, unsigned int w, unsigned int h)
{
    if (fmt == 0x01) return w * h * 4;               /* A8R8G8B8 */
    {
        unsigned int unit = (fmt == 0x1f) ? 8 : 16;  /* DXT1 8B, DXT3 16B / block */
        return ((w + 3) / 4) * ((h + 3) / 4) * unit;
    }
}

int FableDecodeBankTexture(const unsigned char* info,
                           const unsigned char* payload, unsigned int payloadLen,
                           unsigned char* outRGBA, unsigned int outCap,
                           unsigned int* outW, unsigned int* outH)
{
    unsigned int aw, ah, rw, rh, fmt, mips, mip0size, rawlen;
    unsigned char* raw; unsigned int p = 0; int n;

    aw = info[0] | (info[1] << 8);
    ah = info[2] | (info[3] << 8);
    rw = info[6] | (info[7] << 8);
    rh = info[8] | (info[9] << 8);
    fmt = (info[12] | (info[13] << 8) | (info[14] << 16) | ((unsigned int)info[15] << 24)) & 0xFF;
    mips = info[17]; if (mips == 0) mips = 1;
    mip0size = info[24] | (info[25] << 8) | (info[26] << 16) | ((unsigned int)info[27] << 24);
    if (fmt != 0x01 && fmt != 0x1f && fmt != 0x20 && fmt != 0x23 && fmt != 0x18) return -20;
    if (aw == 0 || ah == 0 || aw > 4096 || ah > 4096) return -21;
    if ((unsigned int)aw * ah * 4 > outCap) return -22;

    rawlen = mip_raw_len(fmt, aw, ah);
    raw = (unsigned char*)malloc(rawlen);
    if (!raw) return -23;

    if (mip0size > 0) {
        n = read_chunked(payload, payloadLen, &p, rawlen, raw, rawlen);
        if (n < 0) { free(raw); return -30 + (n < -9 ? -9 : n); }
    } else {
        if (rawlen > payloadLen) { free(raw); return -24; }
        memcpy(raw, payload, rawlen);
        n = (int)rawlen;
    }
    if ((unsigned int)n < rawlen) memset(raw + n, 0, rawlen - n);

    if (fmt == 0x01)            decode_argb(raw, aw, ah, outRGBA);
    else if (fmt == 0x1f)       decode_dxt(raw, aw, ah, 0, outRGBA);
    else                        decode_dxt(raw, aw, ah, 1, outRGBA);

    free(raw);
    if (outW) *outW = rw ? rw : aw;
    if (outH) *outH = rh ? rh : ah;
    return 0;
}
