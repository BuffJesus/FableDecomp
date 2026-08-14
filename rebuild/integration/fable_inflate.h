/* fable_inflate.h — self-contained RFC-1951 DEFLATE / RFC-1950 zlib inflate.
 *
 * The visual-checkpoint / frontend build links no zlib, but Fable .sav HEADER
 * chunks are stored as zlib streams (0x78 0xDA). This decoder inflates them with
 * no external dependency. Derived from Mark Adler's public-domain puff.c.
 *
 * FableZlibInflate: input is a full RFC-1950 stream (2-byte header + DEFLATE +
 * 4-byte Adler32 trailer, trailer ignored). Returns 0 on success and writes the
 * inflated length to *outLen; negative on any error (bad header, overrun of
 * dstCap, malformed stream). Bounds every write by dstCap.
 */
#ifndef FABLE_INFLATE_H
#define FABLE_INFLATE_H

#ifdef __cplusplus
extern "C" {
#endif

/* raw DEFLATE (no zlib wrapper) */
int FableRawInflate(const unsigned char* src, unsigned int srcLen,
                    unsigned char* dst, unsigned int dstCap, unsigned int* outLen);

/* RFC-1950 zlib wrapper around FableRawInflate */
int FableZlibInflate(const unsigned char* src, unsigned int srcLen,
                     unsigned char* dst, unsigned int dstCap, unsigned int* outLen);

#ifdef __cplusplus
}
#endif

#endif /* FABLE_INFLATE_H */
