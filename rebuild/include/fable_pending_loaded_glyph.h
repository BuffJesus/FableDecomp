#pragma once
#include "fable_counted_pointer.h"
class CBankFileAsyncData;
// Ego_r.pdb PendingLoadedGlyph; retail font preload and shutdown confirm offsets.
struct PendingLoadedGlyph {
    CCountedPointer<CBankFileAsyncData> data;
    bool big;
    unsigned long index;
};
FABLE_STATIC_ASSERT(sizeof(PendingLoadedGlyph)==0x10);
FABLE_STATIC_ASSERT(offsetof(PendingLoadedGlyph,big)==8);
FABLE_STATIC_ASSERT(offsetof(PendingLoadedGlyph,index)==12);
// Global names not recovered. Native pointer targets the 512-entry array at 013c82f8.
extern PendingLoadedGlyph* FablePendingGlyphs_0138e074;
extern const unsigned long FablePendingGlyphCount_0129ab30;
extern unsigned long FablePendingGlyphRead_013c5a74;
extern unsigned long FablePendingGlyphWrite_013c5a78;
