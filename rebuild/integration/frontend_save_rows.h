/* frontend_save_rows.h — live save-metadata feeder for the frontend Load Game list.
 *
 * Reads a profile's Profile.bin registry + up to four .sav HEADERs from disk and
 * pushes real row labels + recovered actions into the renderer via
 * FableSetVisualFrontendSaveRows (declared in fable_visual_d3d9.h). This is the
 * native port of tools/save_metadata.py; it closes the seam between the proven
 * python read-contract and the renderer sink that previously only received the
 * authored visual-checkpoint defaults. See docs/SAVE_ROW_FEEDER_SPEC.md.
 *
 * Separate from save_metadata_runtime_boundary.cpp, which reconstructs the boot
 * .MET-from-.LUG generator (0x00418c3b) — a different retail seam.
 */
#ifndef FRONTEND_SAVE_ROWS_H
#define FRONTEND_SAVE_ROWS_H

#include "rebuild_abi.h"
#include <wchar.h>   /* wchar_t under VC7.1 (no /Zc:wchar_t) */

#ifdef __cplusplus
extern "C" {
#endif

/* Recovered row action codes (match save_metadata.py). */
#define FABLE_SAVE_ACTION_LOADABLE 0x11u  /* file exists, container + HEADER valid */
#define FABLE_SAVE_ACTION_INVALID  0xDCu  /* missing file or corrupt/unreadable    */

/* One registry slot: index 1..50 and its filename (== display label). */
typedef struct FableSaveSlot {
    int  index;
    char name[128];
} FableSaveSlot;

/* One assembled row for the frontend. `name` is the retail-faithful display label
 * ("AutoSave" / "Save N"); `filename` is the on-disk .sav it maps to. */
typedef struct FableSaveRowOut {
    char      name[128];
    char      filename[128];
    fable_u32 action;
} FableSaveRowOut;

/* Production entry point: enumerate the profile dir and push <=4 rows to the
 * renderer sink. profileDir is the absolute Saves\<profileName> directory. When
 * no loadable/known rows are found the authored defaults are restored (the sink
 * is called with null names). */
void FABLE_FASTCALL FableFeedVisualFrontendSaveRows(const wchar_t* profileDir);

/* --- test-only helpers (exercised by rebuild/tests/integration) --- */

/* Parse SaveGameNames1..50 from a whole Profile.bin image. Returns the count of
 * non-empty slots written (<=cap), each with its 1-based registry index. Also
 * verifies the FablePro container + trailer crc0; returns 0 on a bad container. */
unsigned int FableParseProfileRegistryForTest(
    const unsigned char* profileBytes, unsigned int len,
    FableSaveSlot* slots, unsigned int cap);

/* Validate one in-memory FableSave! file the way save_metadata.py does (magic,
 * sig_field==trailer_pos, trailer crc0, chunk0 inflate length, HEADER decode with
 * per-field seed-0 CRC tags and no trailing bytes). Returns 0 if the row would be
 * LOADABLE, a negative code otherwise. */
int FableValidateSaveForTest(const unsigned char* fileBytes, unsigned int fileLen);

/* Assemble the ordered rows for a profile dir (autosave-first then ascending
 * manual slots, dedup, cap). Returns the row count written (<=cap). */
unsigned int FableBuildSaveRowsForTest(
    const wchar_t* profileDir, FableSaveRowOut* rows, unsigned int cap);

#ifdef __cplusplus
}
#endif

#endif /* FRONTEND_SAVE_ROWS_H */
