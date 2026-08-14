/* fable_texture_decode.h — decode a Fable bank texture entry to RGBA at runtime.
 *
 * Pure container-read path (parity/purity: NO baking): given a texture entry's
 * 34-byte Info header and its raw payload (as returned by fable_bank_reader
 * FindEntry), this reconstructs mip 0 as 8-bit RGBA. It ports the engine's
 * chunked-LZO1X decompressor (lzo1x_decompress @ 0x00c06b90, via
 * tools/lionhead_lz.py) + the Fable [u16 clen] chunk framing + the DXT1/DXT3/
 * A8R8G8B8 block decoders (tools/parse_texture.py), so the native output equals
 * the verified Python decode byte-for-byte.
 *
 * Freestanding on <string.h>/<stdlib.h>; verified against the real frontend.big
 * MINIMAP_*_FRONT_END entries. Used by the saved-games ring preview to load the
 * selected save's region minimap from the game bank at runtime.
 */
#ifndef FABLE_TEXTURE_DECODE_H
#define FABLE_TEXTURE_DECODE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Decode mip 0 of a bank texture entry into a caller RGBA buffer.
 *   info      : 34-byte Info header (CGraphicHeader(28) + CPixelFormatInit(6))
 *   payload   : entry payload bytes; payloadLen its length
 *   outRGBA   : caller buffer, must hold real_w*real_h*4 bytes (alloc dims used
 *               for decode; the real WxH sub-rect is copied out, top-left)
 *   outW/outH : receive the real (authored) width/height
 * Returns 0 on success, negative on malformed input / unsupported format /
 * output-capacity overflow (outCap in bytes bounds every write). */
int FableDecodeBankTexture(const unsigned char* info,
                           const unsigned char* payload, unsigned int payloadLen,
                           unsigned char* outRGBA, unsigned int outCap,
                           unsigned int* outW, unsigned int* outH);

/* Raw LZO1X-1 decompress (engine lzo1x_decompress). Returns bytes written, or
 * a negative error. Bounds writes by dstCap. Exposed for unit tests. */
int FableLzo1xDecompress(const unsigned char* src, unsigned int srcLen,
                         unsigned char* dst, unsigned int dstCap);

#ifdef __cplusplus
}
#endif

#endif /* FABLE_TEXTURE_DECODE_H */
