#pragma once
// Streaming font glyph cache (lib StreamingFontBank.cpp, retail module 0x009A0EC0..0x009A4E40).
// Names and layouts from Ego_r.pdb (ghidra_out/ego_r_pdb.xml, struct_layouts_egor.tsv); the cache types live in
// the unit's anonymous namespace there. Retail globals: the 32x32 glyph cache (StreamingFontCacheBufferData_
// static<32,32>, 512 entries) at 0x013C5A98 and the 64x64 one (<64,64>, 128 entries) at 0x013C7AC8; a glyph's
// BigChar bit selects the latter. Function labels: ghidra_out/labels_streaming_font.tsv.
#include <stddef.h>
#include "rebuild_abi.h"

// Ego_r CStreamingGlyph (4 bytes; a class: ?FindGlyph@CStreamingFontBank@@IBEPBVCStreamingGlyph@@G@Z).
class CStreamingGlyph {
public:
    unsigned long GlyphIndex : 24;
    unsigned long AdjustmentIndex : 8;
};

// Ego_r CStreamingGlyphData (8 bytes).
struct CStreamingGlyphData {
    signed char   OffsetX;
    signed char   OffsetY;
    signed char   Width;
    signed char   Height;
    unsigned long MemOffset : 23;
    unsigned long Advance : 8;
    unsigned long BigChar : 1;
};

// Ego_r CStreamingGlyphAdjustment (2 bytes; a class: ??M@YI_NABVCStreamingGlyphAdjustment@@0@Z).
class CStreamingGlyphAdjustment {
public:
    unsigned char Scale;
    signed char   Baseline;
};

// Ego_r CStreamingGlyphLookup (16 bytes): one way of the 256x8 glyph lookup cache.
struct CStreamingGlyphLookup {
    unsigned short      font : 4;
    unsigned short      cachePos : 12;
    unsigned short      ch;
    CStreamingGlyph     indices;
    CStreamingGlyphData data;
};

// Ego_r GlyphBank (260 bytes): one decompressed bank of 64 glyphs. CStreamingFontBank::GlyphBanks[] point at
// a 16-bit header (bit 15 = LZO-compressed, low 15 bits = compressed size) followed by the bank.
struct GlyphBank {
    unsigned short  FirstGlyph;
    unsigned short  NoGlyphs;
    CStreamingGlyph Glyphs[64];
};

namespace {

// Ego_r `anonymous-namespace'::StreamingFontCacheEntry (16 bytes): a node of a cache buffer's LRU chain.
struct StreamingFontCacheEntry {
    unsigned short older;
    unsigned short newer;
    long           lastUseTime;
    unsigned long  glyphIndex : 24;
    unsigned long  loading : 1;
    unsigned long  cacheIndex;
};

// Ego_r `anonymous-namespace'::StreamingFontCacheBufferData (0x2C bytes).
struct StreamingFontCacheBufferData {
    unsigned long                 width;
    unsigned long                 height;
    unsigned long                 numEntries;
    unsigned long                 entryWidth;
    unsigned long                 entryHeight;
    unsigned long                 entrySize;
    StreamingFontCacheEntry*      entries;
    unsigned long                 newestEntry;
    unsigned long                 oldestEntry;
    StreamingFontCacheBufferData* nextSmaller;
    StreamingFontCacheBufferData* nextBigger;
};

} // namespace

FABLE_STATIC_ASSERT(sizeof(GlyphBank) == 260);
FABLE_STATIC_ASSERT(sizeof(CStreamingGlyph) == 4);
FABLE_STATIC_ASSERT(sizeof(CStreamingGlyphData) == 8);
FABLE_STATIC_ASSERT(sizeof(CStreamingGlyphAdjustment) == 2);
FABLE_STATIC_ASSERT(sizeof(CStreamingGlyphLookup) == 16);
FABLE_STATIC_ASSERT(sizeof(StreamingFontCacheEntry) == 16);
FABLE_STATIC_ASSERT(sizeof(StreamingFontCacheBufferData) == 0x2C);
FABLE_STATIC_ASSERT(offsetof(StreamingFontCacheBufferData, entries) == 0x18);
FABLE_STATIC_ASSERT(offsetof(StreamingFontCacheBufferData, nextBigger) == 0x28);

// Ego_r ?UninitializeStreamingFontTextureCache@@YIXXZ, retail 0x009A1660 (lib StreamingFontBank.cpp).
void FABLE_FASTCALL UninitializeStreamingFontTextureCache();
