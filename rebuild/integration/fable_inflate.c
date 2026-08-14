/* fable_inflate.c — puff-style RFC-1951 DEFLATE + RFC-1950 zlib inflate.
 *
 * Adapted from Mark Adler's puff.c (public domain, zlib distribution). Reduced to
 * what the Fable .sav HEADER path needs: decode a whole in-memory stream into a
 * caller buffer, bounded by dstCap, C89-clean for VC7.1 (no VLAs, no STL, fixed
 * stack tables). Only ~255-byte HEADER chunks are ever inflated here, but the
 * decoder is general (stored / fixed / dynamic blocks).
 */
#include "fable_inflate.h"

#define MAXBITS   15    /* max bits in a Huffman code */
#define MAXLCODES 286   /* max number of literal/length codes */
#define MAXDCODES 30    /* max number of distance codes */
#define MAXCODES  (MAXLCODES + MAXDCODES)
#define FIXLCODES 288   /* number of fixed literal/length codes */

struct state {
    unsigned char* out;      /* output buffer */
    unsigned int   outcap;   /* capacity of out */
    unsigned int   outcnt;   /* bytes written so far */

    const unsigned char* in; /* input buffer */
    unsigned int   inlen;    /* input length */
    unsigned int   incnt;    /* bytes read so far */
    int            bitbuf;   /* bit buffer */
    int            bitcnt;   /* number of bits in bit buffer */
};

struct huffman {
    short* count;   /* number of symbols of each length */
    short* symbol;  /* canonically ordered symbols */
};

static int bits(struct state* s, int need)
{
    long val = s->bitbuf;
    while (s->bitcnt < need) {
        if (s->incnt == s->inlen) return -1;   /* out of input */
        val |= (long)(s->in[s->incnt++]) << s->bitcnt;
        s->bitcnt += 8;
    }
    s->bitbuf = (int)(val >> need);
    s->bitcnt -= need;
    return (int)(val & ((1L << need) - 1));
}

static int stored(struct state* s)
{
    unsigned len;
    /* discard leftover bits in current byte */
    s->bitbuf = 0;
    s->bitcnt = 0;
    if (s->incnt + 4 > s->inlen) return 2;
    len = s->in[s->incnt++];
    len |= s->in[s->incnt++] << 8;
    /* skip one's-complement */
    s->incnt += 2;
    if (s->incnt + len > s->inlen) return 2;
    if (s->outcnt + len > s->outcap) return 1;
    while (len--) s->out[s->outcnt++] = s->in[s->incnt++];
    return 0;
}

static int decode(struct state* s, const struct huffman* h)
{
    int code = 0, first = 0, index = 0, len = 1, count;
    const short* next = h->count + 1;
    while (1) {
        int b = bits(s, 1);
        if (b < 0) return b;
        code |= b;
        count = *next++;
        if (code - count < first)
            return h->symbol[index + (code - first)];
        index += count;
        first += count;
        first <<= 1;
        code <<= 1;
        len++;
        if (len > MAXBITS) return -10;
    }
}

static int construct(struct huffman* h, const short* length, int n)
{
    int symbol, len, left;
    short offs[MAXBITS + 1];

    for (len = 0; len <= MAXBITS; len++) h->count[len] = 0;
    for (symbol = 0; symbol < n; symbol++) h->count[length[symbol]]++;
    if (h->count[0] == n) return 0;   /* no codes -> complete but empty */

    left = 1;
    for (len = 1; len <= MAXBITS; len++) {
        left <<= 1;
        left -= h->count[len];
        if (left < 0) return left;    /* over-subscribed */
    }

    offs[1] = 0;
    for (len = 1; len < MAXBITS; len++)
        offs[len + 1] = offs[len] + h->count[len];

    for (symbol = 0; symbol < n; symbol++)
        if (length[symbol] != 0) h->symbol[offs[length[symbol]]++] = symbol;

    return left;   /* 0 = complete */
}

static int codes(struct state* s, const struct huffman* lencode,
                 const struct huffman* distcode)
{
    static const short lens[29] = {
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
    static const short lext[29] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
    static const short dists[30] = {
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577 };
    static const short dext[30] = {
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };

    int symbol;
    do {
        symbol = decode(s, lencode);
        if (symbol < 0) return symbol;
        if (symbol < 256) {                 /* literal */
            if (s->outcnt == s->outcap) return 1;
            s->out[s->outcnt++] = (unsigned char)symbol;
        } else if (symbol > 256) {          /* length */
            unsigned int len, dist;
            int extra;
            symbol -= 257;
            if (symbol >= 29) return -10;
            extra = bits(s, lext[symbol]);
            if (extra < 0) return extra;
            len = lens[symbol] + extra;

            symbol = decode(s, distcode);
            if (symbol < 0) return symbol;
            extra = bits(s, dext[symbol]);
            if (extra < 0) return extra;
            dist = dists[symbol] + extra;
            if (dist > s->outcnt) return -11;         /* distance too far back */
            if (s->outcnt + len > s->outcap) return 1;
            while (len--) {
                s->out[s->outcnt] = s->out[s->outcnt - dist];
                s->outcnt++;
            }
        }
    } while (symbol != 256);
    return 0;
}

static int fixed_block(struct state* s)
{
    static short lencnt[MAXBITS + 1], lensym[FIXLCODES];
    static short distcnt[MAXBITS + 1], distsym[MAXDCODES];
    static int built = 0;
    static struct huffman lencode, distcode;

    if (!built) {
        int symbol;
        short lengths[FIXLCODES];
        lencode.count = lencnt;  lencode.symbol = lensym;
        distcode.count = distcnt; distcode.symbol = distsym;
        for (symbol = 0; symbol < 144; symbol++) lengths[symbol] = 8;
        for (; symbol < 256; symbol++) lengths[symbol] = 9;
        for (; symbol < 280; symbol++) lengths[symbol] = 7;
        for (; symbol < FIXLCODES; symbol++) lengths[symbol] = 8;
        construct(&lencode, lengths, FIXLCODES);
        for (symbol = 0; symbol < MAXDCODES; symbol++) lengths[symbol] = 5;
        construct(&distcode, lengths, MAXDCODES);
        built = 1;
    }
    return codes(s, &lencode, &distcode);
}

static int dynamic_block(struct state* s)
{
    static const short order[19] = {
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
    int nlen, ndist, ncode, index, err;
    short lengths[MAXCODES];
    short lencnt[MAXBITS + 1], lensym[MAXLCODES];
    short distcnt[MAXBITS + 1], distsym[MAXDCODES];
    struct huffman lencode, distcode;

    lencode.count = lencnt;   lencode.symbol = lensym;
    distcode.count = distcnt; distcode.symbol = distsym;

    nlen  = bits(s, 5) + 257;
    ndist = bits(s, 5) + 1;
    ncode = bits(s, 4) + 4;
    if (nlen > MAXLCODES || ndist > MAXDCODES) return -3;

    for (index = 0; index < ncode; index++) {
        int b = bits(s, 3);
        if (b < 0) return b;
        lengths[order[index]] = (short)b;
    }
    for (; index < 19; index++) lengths[order[index]] = 0;

    err = construct(&lencode, lengths, 19);
    if (err != 0) return -4;

    index = 0;
    while (index < nlen + ndist) {
        int symbol = decode(s, &lencode);
        if (symbol < 0) return symbol;
        if (symbol < 16) {
            lengths[index++] = (short)symbol;
        } else {
            int len = 0;
            if (symbol == 16) {
                if (index == 0) return -5;
                len = lengths[index - 1];
                symbol = 3 + bits(s, 2);
            } else if (symbol == 17) {
                symbol = 3 + bits(s, 3);
            } else {
                symbol = 11 + bits(s, 7);
            }
            if (index + symbol > nlen + ndist) return -6;
            while (symbol--) lengths[index++] = (short)len;
        }
    }

    if (lengths[256] == 0) return -9;  /* no end-of-block code */

    err = construct(&lencode, lengths, nlen);
    if (err && (err < 0 || nlen != lencode.count[0] + lencode.count[1])) return -7;
    err = construct(&distcode, lengths + nlen, ndist);
    if (err && (err < 0 || ndist != distcode.count[0] + distcode.count[1])) return -8;

    return codes(s, &lencode, &distcode);
}

int FableRawInflate(const unsigned char* src, unsigned int srcLen,
                    unsigned char* dst, unsigned int dstCap, unsigned int* outLen)
{
    struct state s;
    int last, type, err;

    s.out = dst; s.outcap = dstCap; s.outcnt = 0;
    s.in = src;  s.inlen = srcLen;  s.incnt = 0;
    s.bitbuf = 0; s.bitcnt = 0;

    do {
        last = bits(&s, 1);
        type = bits(&s, 2);
        if (last < 0 || type < 0) return -2;
        if (type == 0)      err = stored(&s);
        else if (type == 1) err = fixed_block(&s);
        else if (type == 2) err = dynamic_block(&s);
        else                return -1;          /* reserved block type */
        if (err != 0) return err > 0 ? -100 - err : err;
    } while (!last);

    if (outLen) *outLen = s.outcnt;
    return 0;
}

int FableZlibInflate(const unsigned char* src, unsigned int srcLen,
                     unsigned char* dst, unsigned int dstCap, unsigned int* outLen)
{
    unsigned int cmf, flg;
    if (srcLen < 2) return -20;
    cmf = src[0];
    flg = src[1];
    if ((cmf & 0x0F) != 8) return -21;          /* not DEFLATE */
    if (((cmf << 8) | flg) % 31 != 0) return -22; /* header checksum */
    if (flg & 0x20) return -23;                 /* preset dictionary unsupported */
    /* skip 2-byte header; ignore trailing 4-byte Adler32 */
    return FableRawInflate(src + 2, srcLen - 2, dst, dstCap, outLen);
}
