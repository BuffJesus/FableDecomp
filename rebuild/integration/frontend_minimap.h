/* frontend_minimap.h — runtime region-minimap provider for the saved-games ring.
 *
 * Parity/purity: loads the selected save's region minimap from the real game bank
 * (frontend.big, sub-bank GBANK_FRONT_END_PC) at RUNTIME and decodes it to RGBA,
 * exactly as retail's UI_RING_PIC_DRAW_FROM_VIEWPORT reads
 * CurrentRegionMinimapGraphicName. No build-time baking. Ties together
 * fable_bank_reader (Open/FindEntry+Info) and fable_texture_decode (LZO1X+DXT3).
 *
 * The bank file is opened once and cached across calls (the ring reloads on every
 * selection change; ~4 rows). Call FableReleaseRegionMinimapBank at shutdown.
 */
#ifndef FRONTEND_MINIMAP_H
#define FRONTEND_MINIMAP_H

#include "rebuild_abi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Load the region minimap named by a save's CurrentRegionMinimapGraphicName into
 * outRGBA (needs w*h*4 bytes; 256x256 for all shipped minimaps). `graphicName`
 * is the HEADER value (e.g. "MINIMAP_GREATWOOD"); the provider appends
 * "_FRONT_END" when absent. `bankPath` is the frontend.big path. Returns 0 on
 * success (outW/outH set), negative on missing bank/entry/decode error. */
int FableLoadRegionMinimap(const char* bankPath, const char* graphicName,
                           unsigned char* outRGBA, unsigned int outCap,
                           unsigned int* outW, unsigned int* outH);

/* Release the cached bank (frees the in-memory frontend.big). */
void FableReleaseRegionMinimapBank(void);

#ifdef __cplusplus
}
#endif

#endif /* FRONTEND_MINIMAP_H */
