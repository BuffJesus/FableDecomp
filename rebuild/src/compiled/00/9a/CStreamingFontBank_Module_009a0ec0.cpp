// lib StreamingFontBank.cpp, retail module 0x009A0EC0..0x009A4E40 (names from Ego_r.pdb by module order and
// shape; ghidra_out/labels_streaming_font.tsv). One unit, as in retail, because the glyph-cache helpers live
// in its anonymous namespace. Every function of the module is reconstructed: the alpha-mask/fixed-function
// statics, cache-entry LRU helpers, statics, destructor and cache teardown (exact roots:
// gate_native_streaming_font_cache.py); FindGlyph, GlyphLookup, GetStringExtents, cache init, constructor,
// Preload, texture-cache update, text-file reads, PrepareToRender and Render (behaviour against transplanted
// retail: gate_native_streaming_font_glyphs.py).
// Retail calls the allocator's Enter/LeaveSynchronisation out of line here (0x009F2000 / 0x009F2010).
#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
// <new> declares operator new[] / delete[]; without it VC7.1 sends array new/delete of POD to new/delete.
#include <new>
#include <math.h>
#include "fable_font_instances.h"
#include "fable_streaming_font_cache.h"
#include "fable_2d_vector.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"
#include "fable_font_data_banks.h"
#include "fable_memory_allocator.h"
#include "fable_memory_allocator_deallocate.h"
#include "fable_surface_types.h"
#include "fable_carray.h"
#include "fable_data_input_stream.h"
#include "fable_game_text_bank.h"
#include "fable_disk_file.h"
#include "fable_render_state_defaults.h"
#include "fable_shader_render_manager.h"
#include "fable_4d_vector.h"

// Retail 0x013BC470: the process-wide shader manager; Render reads VertexShadersEnabled (+0x390) and
// PixelShadersEnabled (+0x391).
extern CShaderRenderManager g_CShaderRenderManager_013BC470;

C2DVector CStreamingFontBank::AlphaMaskUVMul;  // retail 0x013CA2F8
C2DVector CStreamingFontBank::AlphaMaskUVAdd;  // retail 0x013CA300
bool CStreamingFontBank::UseFixedFunction;     // retail 0x013C5A7C

// Retail 0x009A0EC0.
void FABLE_FASTCALL CStreamingFontBank::SetAlphaMaskUVs(const C2DVector& mul, const C2DVector& add)
{
    AlphaMaskUVMul = mul;
    AlphaMaskUVAdd = add;
}

// Retail 0x009A0EF0.
void FABLE_FASTCALL CStreamingFontBank::SetUseFixedFunction(bool useFixedFunction)
{
    UseFixedFunction = useFixedFunction;
}

namespace {

__forceinline long CurrentFrame()
{
    return GFGetSystemManager()->PDisplayManager.Data->FrameCounter;
}

// Retail 0x009A0F00: on a fresh buffer (newest == oldest), chain every entry oldest-last, mark it empty and
// old enough to be reused at once.
void FABLE_FASTCALL InitializeStreamingFontCacheEntries(StreamingFontCacheBufferData& data)
{
    if (data.newestEntry != data.oldestEntry)
        return;
    const long frame = CurrentFrame();
    StreamingFontCacheEntry* first = data.entries;
    first->older = 1;
    first->newer = (unsigned short)data.numEntries;
    first->lastUseTime = frame - 0x40000000;
    first->glyphIndex = 0;
    data.newestEntry = 0;
    for (unsigned short i = 1; i < data.numEntries; ++i)
    {
        StreamingFontCacheEntry& entry = data.entries[i];
        entry.older = i + 1;
        entry.newer = i - 1;
        entry.lastUseTime = frame - 0x40000000;
        entry.glyphIndex = 0;
    }
    data.oldestEntry = data.numEntries - 1;
}

// Retail 0x009A0F90: move an entry to the newest end of the chain (once per frame).
void FABLE_FASTCALL UpdateUseStreamingFontCacheEntry(StreamingFontCacheBufferData& data, unsigned short index)
{
    StreamingFontCacheEntry& entry = data.entries[index];
    const long frame = CurrentFrame();
    if (entry.lastUseTime == frame)
        return;
    entry.lastUseTime = frame;
    if (entry.newer < data.numEntries)
        data.entries[entry.newer].older = entry.older;
    else
        data.newestEntry = entry.older;
    if (entry.older < data.numEntries)
        data.entries[entry.older].newer = entry.newer;
    else
        data.oldestEntry = entry.newer;
    entry.newer = (unsigned short)data.numEntries;
    entry.older = (unsigned short)data.newestEntry;
    data.entries[entry.older].newer = index;
    data.newestEntry = index;
}

// Retail 0x009A1020: recycle the oldest entry as the newest and return its index.
unsigned short FABLE_FASTCALL InsertNewStreamingFontCacheEntry(StreamingFontCacheBufferData& data)
{
    const unsigned short index = (unsigned short)data.oldestEntry;
    StreamingFontCacheEntry& entry = data.entries[index];
    entry.lastUseTime = CurrentFrame();
    data.entries[entry.newer].older = (unsigned short)data.numEntries;
    data.oldestEntry = entry.newer;
    entry.newer = (unsigned short)data.numEntries;
    entry.older = (unsigned short)data.newestEntry;
    data.entries[entry.older].newer = index;
    data.newestEntry = index;
    return index;
}

// Ego_r StreamingFontCacheBufferData_static<W,H>: a cache buffer over the shared 1024x512 glyph pixel cache
// with its own entry storage, one entry per WxH cell (512 for 32x32, 128 for 64x64), linked to the buffers of
// the next smaller and bigger cell size (their cells overlap). Constructed by the unit's static initialisers
// (retail 0x01223F00 / 0x01223F60).
template<unsigned long W, unsigned long H>
struct StreamingFontCacheBufferData_static : StreamingFontCacheBufferData {
    StreamingFontCacheEntry entries[(1024 / W) * (512 / H)];
    StreamingFontCacheBufferData_static(StreamingFontCacheBufferData* smaller, StreamingFontCacheBufferData* bigger)
    {
        width = 1024 / W;
        height = 512 / H;
        numEntries = (1024 / W) * (512 / H);
        entryWidth = W;
        entryHeight = H;
        entrySize = W * H;
        StreamingFontCacheBufferData::entries = entries;
        newestEntry = 0;
        oldestEntry = 0;
        nextSmaller = smaller;
        nextBigger = bigger;
    }
};

// The two cache buffers (Ego_r file statics g_StreamingFontCacheBufferData / ...2; unnamed namespace so the pair
// can name each other).
extern StreamingFontCacheBufferData_static<32, 32> g_StreamingFontCacheBufferData;   // 0x013C5A98
extern StreamingFontCacheBufferData_static<64, 64> g_StreamingFontCacheBufferData2;  // 0x013C7AC8

} // namespace

extern "C" int __cdecl lzo1x_decompress_safe(const unsigned char* src, unsigned long srcLength,
                                             unsigned char* dst, unsigned long* dstLength, void* workMemory);

// File statics (Ego_r names; retail addresses).
static unsigned long numTextureCacheGlyphDatasPerChunk = 0x40;           // 0x0138E068
static unsigned long glyphTextureCacheGlyphDataCacheChunk = 0xFFFFFFFF;  // 0x0138E06C
static long GFontTextureBytesPerPixel = 2;                               // 0x0138E070
static unsigned char* textureCacheMem;                                   // 0x013BDA24
static unsigned long textureCacheSize;                                   // 0x013BDA28
static unsigned long glyphTextureCacheGlyphDataSize;                     // 0x013BDA2C
static unsigned long glyphTextureCacheGlyphDataNumIndices;               // 0x013BDA30
static unsigned long* glyphTextureCacheGlyphDataIndices;                 // 0x013BDA34
static void* glyphTextureCacheGlyphData;                                 // 0x013BDA38
static CStreamingGlyphData* glyphTextureCacheGlyphDataCache;             // 0x013BDA3C
static CStreamingGlyphAdjustment* glyphTextureCacheGlyphAdjustment;      // 0x013BDA40
static unsigned long textureCacheNumGlyphs;                              // 0x013BDA44
static unsigned long textureCacheNumGlyphAdjustments;                    // 0x013BDA48
static unsigned long textureCacheMemOffset;                              // 0x013BDA4C
static CStreamingFontBank* streamingFontBankList[8];                    // 0x013BDA50
static unsigned char GFontTextureCacheUpdateMask[512];                  // 0x013BD808 (a cache cell's pixels changed)
static bool GFontTextureCacheUpdateFlag;                                // 0x013C5A70 (any cell changed)

// Ego_r PendingLoadedGlyph (16 bytes): a glyph's pixels being streamed into a cache cell.
struct PendingLoadedGlyph {
    CCountedPointer<CBankFileAsyncData> data;
    bool                                big;
    unsigned long                       index;
};

// Ego_r NStreamingFontBankGlobals: the ring of reads in flight (head = oldest, tail = next free).
namespace NStreamingFontBankGlobals {
PendingLoadedGlyph g_PendingLoadedGlyphs[512];                          // 0x013C82F8
unsigned long g_HeadPendingLoadedGlyphs;                                // 0x013C5A74
unsigned long g_TailPendingLoadedGlyphs;                                // 0x013C5A78
}
using namespace NStreamingFontBankGlobals;

template<> inline void CCountedPointer<CBankFileAsyncData>::ReleaseData()
{
    Release();
    Info = 0;
    Data = 0;
}
static CTexture GFontTextureCache;                                       // 0x013C5A80
static CArray<unsigned char> GFontPixelLookupTable;                      // 0x013C5A88

// STLport's resize is an in-class inline that VC7.1 keeps out of line in some units; in this one retail expands
// it (cut the tail, or fill-insert zeroes through vector<unsigned char>::_M_fill_insert 0x00485F3A).
namespace _STL {
template<> __forceinline void vector<unsigned char>::resize(size_t __new_size, const unsigned char& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}
template<> __forceinline void vector<unsigned char>::resize(size_t __new_size)
{
    resize(__new_size, (unsigned char)0);
}
template<> __forceinline void vector<char>::resize(size_t __new_size, const char& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}
template<> __forceinline void vector<char>::resize(size_t __new_size)
{
    resize(__new_size, (char)0);
}
}
static CStreamingGlyphLookup glyphLookup[256][8];                        // 0x013BDA70
namespace {
StreamingFontCacheBufferData_static<32, 32> g_StreamingFontCacheBufferData(0, &g_StreamingFontCacheBufferData2);
StreamingFontCacheBufferData_static<64, 64> g_StreamingFontCacheBufferData2(&g_StreamingFontCacheBufferData, 0);
}

namespace {

// The cache buffer a glyph lives in: 64x64 for big characters, else 32x32.
__forceinline StreamingFontCacheBufferData& GetCacheBuffer(bool bigChar)
{
    return bigChar ? static_cast<StreamingFontCacheBufferData&>(g_StreamingFontCacheBufferData2)
                   : static_cast<StreamingFontCacheBufferData&>(g_StreamingFontCacheBufferData);
}

// Ego_r ?IsGlyphDirty@?A0xfe46a155@@YI_N_NKK@Z (inlined in retail): the cache slot no longer holds this glyph.
__forceinline bool FABLE_FASTCALL IsGlyphDirty(bool bigChar, unsigned long cachePos, unsigned long glyphIndex)
{
    return GetCacheBuffer(bigChar).entries[cachePos].glyphIndex != glyphIndex;
}

} // namespace

// A GlyphBanks[] record: 16-bit header, then the bank (LZO-compressed when the top bit is set).
struct StoredGlyphBank {
    unsigned short size : 15;
    unsigned short compressed : 1;
    unsigned char  data[1];
};

// Retail 0x009A4160: a glyph's index record. Banks of 64 characters may be LZO-compressed; the last one used
// is decompressed into the font's bank cache.
const CStreamingGlyph* CStreamingFontBank::FindGlyph(unsigned short ch) const
{
    if (ch == 0xA0)
        ch = 0x20;
    GlyphBank* bank = glyphBankCache;
    const unsigned long bankIndex = ch >> 6;
    if (glyphBankCacheIndex != bankIndex)
    {
        StoredGlyphBank* stored = static_cast<StoredGlyphBank*>(GlyphBanks[bankIndex]);
        if (!stored)
            return 0;
        if (stored->compressed)
        {
            unsigned long size = sizeof(GlyphBank);
            lzo1x_decompress_safe(stored->data, stored->size, reinterpret_cast<unsigned char*>(bank), &size, 0);
            glyphBankCacheIndex = bankIndex;
        }
        else
            bank = reinterpret_cast<GlyphBank*>(stored->data);
    }
    const unsigned long index = (ch & 63) - bank->FirstGlyph;
    if (index < bank->NoGlyphs && bank->Glyphs[index].GlyphIndex)
        return &bank->Glyphs[index];
    return 0;
}

// Retail 0x009A1140: the lookup-cache way for a character of this font, filling a free, stale or least
// recently used way on a miss. Characters the font lacks become U+25A1 (white square), or null.
CStreamingGlyphLookup* CStreamingFontBank::GlyphLookup(unsigned short ch) const
{
    while (!(GlyphFlags[ch >> 5] & (1 << (ch & 31))))
    {
        if (ch == 0x25A1)
            return 0;
        ch = 0x25A1;
    }
    const unsigned long bucket = ch & 0xFF;
    CStreamingGlyphLookup* ways = glyphLookup[bucket];
    unsigned long way;
    for (way = 0; way < 8; ++way)
        if (ways[way].ch == ch && ways[way].font == fontIndex)
            break;
    if (way == 8)
    {
    for (way = 0; way < 8; ++way)
        if (ways[way].ch == 0)
            break;
    if (way == 8)
    {
        for (way = 0; way < 8; ++way)
            if (IsGlyphDirty(ways[way].data.BigChar != 0, ways[way].cachePos, ways[way].indices.GlyphIndex))
                break;
        if (way == 8)
        {
            const long frame = CurrentFrame();
            unsigned long oldest = 0;
            way = 0;
            for (unsigned long i = 0; i < 8; ++i)
            {
                const unsigned long age = frame - GetCacheBuffer(ways[i].data.BigChar != 0).entries[ways[i].cachePos].lastUseTime;
                if (oldest <= age)
                {
                    way = i;
                    oldest = age;
                }
            }
            if (way == 8)
                return 0;
        }
    }
    const CStreamingGlyph* glyph = FindGlyph(ch);
    const unsigned long dataIndex = glyph->GlyphIndex - 1;
    const unsigned long chunk = dataIndex / numTextureCacheGlyphDatasPerChunk;
    const unsigned long offset = dataIndex % numTextureCacheGlyphDatasPerChunk;
    if (glyphTextureCacheGlyphDataCacheChunk != chunk)
    {
        unsigned long size = numTextureCacheGlyphDatasPerChunk * sizeof(CStreamingGlyphData);
        const unsigned long start = glyphTextureCacheGlyphDataIndices[chunk];
        lzo1x_decompress_safe(static_cast<const unsigned char*>(glyphTextureCacheGlyphData) + start,
                              glyphTextureCacheGlyphDataIndices[chunk + 1] - start,
                              reinterpret_cast<unsigned char*>(glyphTextureCacheGlyphDataCache), &size, 0);
        glyphTextureCacheGlyphDataCacheChunk = chunk;
    }
    CStreamingGlyphLookup& entry = glyphLookup[bucket][way];
    entry.ch = ch;
    entry.font = fontIndex;
    entry.cachePos = 0;
    entry.indices = *glyph;
    entry.data = glyphTextureCacheGlyphDataCache[offset];
    }
    return &glyphLookup[bucket][way];
}

namespace {

// Retail 0x009A23E0 (Ego_r ?GFUpdateFontTextureCache@?A0xfe46a155@@YIXXZ): copy every changed 32x32 cell of the
// glyph pixel cache (alpha bytes) into the font texture cache through the alpha-to-texel lookup table.
void FABLE_FASTCALL GFUpdateFontTextureCache()
{
    CSurfaceLockedRect locked = GFontTextureCache.Lock(0, 0);
    for (unsigned long cell = 0, offset = 0; offset < 0x80000; offset += 0x400, ++cell)
    {
        if (!GFontTextureCacheUpdateMask[cell])
            continue;
        const unsigned char* source = textureCacheMem + offset;
        const unsigned long column = cell & 31;
        unsigned long row = (cell >> 5) << 5;
        for (unsigned long line = 0; line < 32; ++line, ++row)
        {
            unsigned char* texels = locked.PMem + row * locked.Pitch + column * GFontTextureBytesPerPixel * 32;
            if (GFontTextureBytesPerPixel == 2)
            {
                const unsigned short* lookup = reinterpret_cast<const unsigned short*>(GFontPixelLookupTable.GetAsCArray());
                unsigned short* texel = reinterpret_cast<unsigned short*>(texels);
                for (int i = 0; i < 32; ++i)
                    *texel++ = lookup[*source++];
            }
            else if (GFontTextureBytesPerPixel == 1)
            {
                const unsigned char* lookup = GFontPixelLookupTable.GetAsCArray();
                for (int i = 0; i < 32; ++i)
                    *texels++ = lookup[*source++];
            }
            else
            {
                for (int i = 0; i < 32; ++i)
                {
                    memcpy(texels, &GFontPixelLookupTable[0] + *source++ * GFontTextureBytesPerPixel, GFontTextureBytesPerPixel);
                    texels += GFontTextureBytesPerPixel;
                }
            }
        }
        GFontTextureCacheUpdateMask[cell] = 0;
    }
    GFontTextureCache.Unlock(0);
    GFontTextureCacheUpdateFlag = false;
}

} // namespace

namespace {

// Retail 0x009A3460 (Ego_r ?ReadTextFileIntoArray@?A0xfe46a155@@YIKABVCWideString@@AAV?$CArray@G@@@Z): read a
// text file as UTF-16 (little- or big-endian by its byte-order mark, which is dropped) or as 8-bit characters,
// zero-terminated; returns the character count.
unsigned long FABLE_FASTCALL ReadTextFileIntoArray(const CWideString& filename, CArray<unsigned short>& text)
{
    CDiskFileWin32 file;
    file.Open(filename, FILE_ACCESS_READ, 2);
    unsigned long count = file.GetLength();
    CArray<char> bytes;
    bytes.resize(count + 1);
    file.Read(bytes.GetAsCArray(), count, 0);
    file.Close();
    if (bytes[0] == (char)0xFF && bytes[1] == (char)0xFE)
    {
        count = count / 2 - 1;
        text.resize(count + 1);
        for (unsigned long i = 0; i < count; ++i)
            text[i] = (unsigned short)(((unsigned char)bytes[i * 2 + 3] << 8) + (unsigned char)bytes[i * 2 + 2]);
    }
    else if (bytes[0] == (char)0xFE && bytes[1] == (char)0xFF)
    {
        count = count / 2 - 1;
        text.resize(count + 1);
        for (unsigned long i = 0; i < count; ++i)
            text[i] = (unsigned short)(((unsigned char)bytes[i * 2 + 2] << 8) + (unsigned char)bytes[i * 2 + 3]);
    }
    else
    {
        text.resize(count + 1);
        for (unsigned long i = 0; i < count; ++i)
            text[i] = (unsigned char)bytes[i];
    }
    text[count] = 0;
    return count;
}

// Retail 0x009A3610: the same for a narrow file name.
unsigned long FABLE_FASTCALL ReadTextFileIntoArray(const CCharString& filename, CArray<unsigned short>& text)
{
    return ReadTextFileIntoArray(filename.ToWideString(), text);
}

} // namespace

// Retail 0x009A2000: make the text's glyphs resident in the pixel cache. Finished reads are retired first; each
// glyph not in its cache cell gets the oldest cell, cleared, and a read of its pixels (asynchronous when asked
// for); cells of the other sizes that overlap the glyph's cell are invalidated. True when every glyph was
// already resident.
bool CStreamingFontBank::Preload(const CWideString& text, bool asynchronous) const
{
    bool resident = true;
    CFontManager* fonts = GFGetSystemManager()->PFontManager.Data;
    while (g_HeadPendingLoadedGlyphs != g_TailPendingLoadedGlyphs)
    {
        PendingLoadedGlyph& pending = g_PendingLoadedGlyphs[g_HeadPendingLoadedGlyphs];
        if (pending.data.Data && !pending.data->IsFinished())
            break;
        GetCacheBuffer(pending.big).entries[pending.index].loading = 0;
        pending.data.ReleaseData();
        GFontTextureCacheUpdateMask[pending.index] = 1;
        GFontTextureCacheUpdateFlag = true;
        --fonts->NumberAsyncGlyphsPending;
        g_HeadPendingLoadedGlyphs = (g_HeadPendingLoadedGlyphs + 1) & 0x1FF;
    }
    const bool wasAsynchronous = fonts->LoadFontGlyphsAsynchronously;
    fonts->LoadFontGlyphsAsynchronously = asynchronous;
    for (const wchar_t* p = text.GetAsCharArray(); *p; ++p)
    {
        const wchar_t ch = *p;
        if (ch == 10 || ch == 0x20 || ch == 0xA0)
            continue;
        CStreamingGlyphLookup* lookup = GlyphLookup(ch);
        if (!lookup)
            continue;
        StreamingFontCacheBufferData& cache = GetCacheBuffer(lookup->data.BigChar != 0);
        const unsigned long position = lookup->cachePos;
        if (cache.entries[position].glyphIndex != lookup->indices.GlyphIndex)
        {
            const unsigned short cell = InsertNewStreamingFontCacheEntry(cache);
            lookup->cachePos = cell;
            StreamingFontCacheEntry& entry = cache.entries[cell];
            entry.glyphIndex = lookup->indices.GlyphIndex;
            entry.loading = 1;
            unsigned char* pixels = textureCacheMem + cache.entrySize * cell;
            memset(pixels, 0, cache.entrySize);
            GFontTextureCacheUpdateFlag = true;
            GFontTextureCacheUpdateMask[cell] = 1;
            PendingLoadedGlyph& pending = g_PendingLoadedGlyphs[g_TailPendingLoadedGlyphs];
            pending.data = data_bank->StreamGlyph(pixels, (lookup->data.MemOffset << 10) + textureCacheMemOffset, cache.entrySize, false);
            pending.big = lookup->data.BigChar != 0;
            pending.index = cell;
            if (pending.data.Data && !pending.data->IsFinished())
            {
                g_TailPendingLoadedGlyphs = (g_TailPendingLoadedGlyphs + 1) & 0x1FF;
                ++fonts->NumberAsyncGlyphsPending;
                resident = false;
            }
            else
                entry.loading = 0;
        }
        else
        {
            if (cache.entries[position].loading)
                resident = false;
            UpdateUseStreamingFontCacheEntry(cache, (unsigned short)position);
        }
        // Cells of the bigger sizes containing this one, and of the smaller sizes inside it, no longer hold glyphs.
        unsigned long x = position % cache.width;
        unsigned long y = position / cache.width;
        for (StreamingFontCacheBufferData* bigger = cache.nextBigger; bigger; bigger = bigger->nextBigger)
        {
            y >>= 1;
            x >>= 1;
            const unsigned long index = bigger->width * y + x;
            UpdateUseStreamingFontCacheEntry(*bigger, (unsigned short)index);
            bigger->entries[index].glyphIndex = 0;
        }
        x = position % cache.width;
        y = position / cache.width;
        unsigned long span = 1;
        for (StreamingFontCacheBufferData* smaller = cache.nextSmaller; smaller; smaller = smaller->nextSmaller)
        {
            x <<= 1;
            y <<= 1;
            span <<= 1;
            for (short row = 0; row < span; ++row)
            {
                for (short column = 0; column < span; ++column)
                {
                    const unsigned long index = smaller->width * (y + row) + column + x;
                    UpdateUseStreamingFontCacheEntry(*smaller, (unsigned short)index);
                    smaller->entries[index].glyphIndex = 0;
                }
            }
        }
    }
    fonts->LoadFontGlyphsAsynchronously = wasAsynchronous;
    return resident;
}

// Retail 0x009A1720: render states for streamed glyphs (alpha blended and alpha tested at 8, depth tested, no
// fog, linear filtering); with texture stages, colour from the vertex and alpha from texture times vertex.
void CStreamingFontBank::PrepareToRender(bool setupTextureStages) const
{
    CRenderManagerCore* render = &GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    render->SetRenderState(1, render->DeviceState.AlphaBlendEnable);
    render->SetRenderState(FableRenderDefaultValue_01396f78, render->DeviceState.SourceAlphaBlendMode);
    render->SetRenderState(FableRenderDefaultValue_01396f7c, render->DeviceState.DestAlphaBlendMode);
    render->SetRenderState(1, render->DeviceState.AlphaTestEnable);
    render->SetRenderState(8, render->DeviceState.AlphaRef);
    render->SetRenderState(CRenderStateManager::D3DAlphaFuncModeTable[ALPHA_TEST_GREATER_EQUAL], render->DeviceState.AlphaTestFunc);
    render->SetRenderState(FableRenderDefaultValue_01396fa4, render->DeviceState.PolyFillMode);
    render->SetRenderState(FableRenderDefaultValue_01396fb0, render->DeviceState.PolyCullMode);
    render->SetRenderState(1, render->DeviceState.ZBufferEnable);
    render->SetRenderState(0, render->DeviceState.FogEnable);
    render->SetRenderState(1, render->DeviceState.ClippingEnable);
    render->SetRenderState(FableRenderDefaultValue_01396e5c, render->DeviceState.TextureFilterModeMin[0]);
    render->SetRenderState(FableRenderDefaultValue_01396e5c, render->DeviceState.TextureFilterModeMag[0]);
    render->SetRenderState(CRenderStateManager::D3DTextureMipMapModeTable[0], render->DeviceState.TextureMipMapMode[0]);
    render->SetRenderState(FableRenderDefaultValue_01396fcc, render->DeviceState.TextureTransformMode[0]);
    if (setupTextureStages)
    {
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_SELECT_ARG1], render->DeviceState.TextureOperation[0]);
        render->SetRenderState(FableRenderDefaultValue_01396eec, render->DeviceState.TextureArgument1[0]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_MODULATE], render->DeviceState.TextureAlphaOperation[0]);
        render->SetRenderState(FableRenderDefaultValue_01396ef0, render->DeviceState.TextureAlphaArgument1[0]);
        render->SetRenderState(FableRenderDefaultValue_01396eec, render->DeviceState.TextureAlphaArgument2[0]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_DISABLE], render->DeviceState.TextureOperation[1]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_DISABLE], render->DeviceState.TextureAlphaOperation[1]);
    }
}

// Retail 0x009A14E0: width of the widest line and height of the text (FontHeight + 1 per line); advances
// are the glyph advance scaled by its adjustment (percent), rounded half away from zero.
C2DExtentsF CStreamingFontBank::GetStringExtents(const wchar_t* text, bool firstLineOnly) const
{
    float maxX = 0.0f;
    float maxY = 0.0f;
    float currentX = 0.0f;
    float currentY = static_cast<float>(FontHeight + 1);
    wchar_t ch;
    while ((ch = *text) != 0)
    {
        ++text;
        if (ch == 10)
        {
            if (firstLineOnly)
                return C2DExtentsF(maxX, maxY);
            currentX = 0.0f;
            currentY += static_cast<float>(FontHeight + 1);
        }
        else if (ch == 0xA0)
            ch = 0x20;
        const CStreamingGlyphLookup* lookup = GlyphLookup(ch);
        if (lookup)
        {
            const float scale = glyphTextureCacheGlyphAdjustment[lookup->indices.AdjustmentIndex].Scale * 0.01f;
            const float advance = scale * static_cast<float>(lookup->data.Advance);
            currentX += static_cast<signed char>(static_cast<long>(advance < 0.0f ? advance - 0.5f : advance + 0.5f));
        }
        if (currentX > maxX)
            maxX = currentX;
        if (currentY > maxY)
            maxY = currentY;
    }
    return C2DExtentsF(maxX, maxY);
}

// Ego_r CFontVertex (28 bytes) and CFontVertexMask (36 bytes: a second UV set for the alpha mask texture).
struct CFontVertex
{
    C4DVector Position;
    unsigned long Colour;
    float TU, TV;
    CFontVertex() {}
    CFontVertex(const C4DVector& position, unsigned long colour, float tu, float tv)
        : Position(position), Colour(colour), TU(tu), TV(tv) {}
};
FABLE_STATIC_ASSERT(sizeof(CFontVertex) == 28);

struct CFontVertexMask
{
    C4DVector Position;
    unsigned long Colour;
    float TU, TV;
    float MaskU, MaskV;
    CFontVertexMask() {}
    CFontVertexMask(const C4DVector& position, unsigned long colour, float tu, float tv, float maskU, float maskV)
        : Position(position), Colour(colour), TU(tu), TV(tv), MaskU(maskU), MaskV(maskV) {}
};
FABLE_STATIC_ASSERT(sizeof(CFontVertexMask) == 36);

// CRenderManagerCore::AttachTextureToStage (0x009A0CF0), inlined into Render in retail.
static __forceinline void AttachTexture(CRenderManagerCore* render, long stage, const CTexture* texture)
{
    IDirect3DBaseTexture9* deviceTexture = texture != 0 ? texture->PD3DTexture : 0;
    if (render->CurrentlySelectedTextures[stage] != deviceTexture)
    {
        render->CurrentlySelectedTextures[stage] = deviceTexture;
        typedef HRESULT (__stdcall* SetTextureCall)(IDirect3DDevice9*, unsigned long, IDirect3DBaseTexture9*);
        void** vtable = *reinterpret_cast<void***>(render->PD3DDevice);
        reinterpret_cast<SetTextureCall>(vtable[65])(render->PD3DDevice, stage, deviceTexture);
    }
    if (render->TextureStagesInUse < stage + 1)
        render->TextureStagesInUse = stage + 1;
}

// Retail 0x009A25D0: draw the text from the glyph texture cache as indexed quads, 64 per batch. Flags 2/4
// right-align/centre each line on the position, 8 centres the block vertically. With a mask texture and the
// fixed-function vertex path, vertices carry a second UV set (the position in render-target clip space
// through AlphaMaskUVMul/Add) and stage 1 modulates the alpha by the mask.
void CStreamingFontBank::Render(const CWideString& text, const C3DVector& position, float scale,
    const CRGBColour& colour, unsigned long flags, const CTexture* texture) const
{
    if (!Preload(text, GFGetSystemManager()->PFontManager.Data->GetLoadFontGlyphsAsynchronously()))
        return;
    const float anchorX = position.X;
    const float anchorY = position.Y;
    float penX = anchorX;
    float penY = anchorY;
    const unsigned long packedColour = *reinterpret_cast<const unsigned long*>(&colour);
    CRenderManagerCore* render = &GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    if (GFontTextureCacheUpdateFlag)
        GFUpdateFontTextureCache();
    if (flags & 8)
        penY = static_cast<float>(floor(anchorY - GetStringExtents(text.GetAsCharArray(), false).Y * 0.5f));
    long stride = sizeof(CFontVertex);
    bool mask = false;
    if (!g_CShaderRenderManager_013BC470.VertexShadersEnabled)
    {
        if (texture)
        {
            render->SetVertexFormat(0x244);
            stride = sizeof(CFontVertexMask);
            mask = true;
        }
        else
            render->SetVertexFormat(0x144);
    }
    CFontVertexMask vertices[256];
    CFontVertexMask* nextMask = vertices;
    CFontVertex* next = reinterpret_cast<CFontVertex*>(vertices);
    static bool buildIndices = true;
    static unsigned short indices[64 * 6];
    if (buildIndices)
    {
        unsigned short* index = indices;
        for (unsigned long quad = 0; quad < 64; ++quad)
        {
            const unsigned short first = static_cast<unsigned short>(quad * 4);
            *index++ = first;
            *index++ = first + 1;
            *index++ = first + 2;
            *index++ = first + 2;
            *index++ = first + 3;
            *index++ = first;
        }
        buildIndices = false;
    }
    AttachTexture(render, 0, &GFontTextureCache);
    AttachTexture(render, 1, texture);
    if (texture && !g_CShaderRenderManager_013BC470.PixelShadersEnabled)
    {
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_SELECT_ARG1], render->DeviceState.TextureOperation[1]);
        render->SetRenderState(FableRenderDefaultValue_01396ee8, render->DeviceState.TextureArgument1[1]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_MODULATE], render->DeviceState.TextureAlphaOperation[1]);
        render->SetRenderState(FableRenderDefaultValue_01396ef0, render->DeviceState.TextureAlphaArgument1[1]);
        render->SetRenderState(FableRenderDefaultValue_01396ee8, render->DeviceState.TextureAlphaArgument2[1]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_DISABLE], render->DeviceState.TextureOperation[2]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_DISABLE], render->DeviceState.TextureAlphaOperation[2]);
    }
    const C2DExtentsI target = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();
    unsigned long numVertices = 0;
    long lineStart = 1;
    const float clipScaleX = 1.0f / (target.X * 0.5f);
    const float clipScaleY = 1.0f / (target.Y * 0.5f);
    const wchar_t* chars = text.GetAsCharArray();
    while (*chars)
    {
        if (lineStart)
        {
            if (flags & 6)
            {
                const C2DExtentsF extents = GetStringExtents(chars, true);
                if (flags & 2)
                    penX = static_cast<float>(floor(anchorX - extents.X));
                if (flags & 4)
                    penX = static_cast<float>(floor(anchorX - extents.X * 0.5f));
            }
            lineStart = 0;
        }
        wchar_t ch = *chars++;
        if (ch == 10)
        {
            penX = anchorX;
            penY += static_cast<float>(FontHeight);
            lineStart = 1;
            continue;
        }
        if (ch == 0xA0)
            ch = 0x20;
        const CStreamingGlyphLookup* lookup = GlyphLookup(ch);
        if (!lookup)
            continue;
        const CStreamingGlyphAdjustment& adjustment = glyphTextureCacheGlyphAdjustment[lookup->indices.AdjustmentIndex];
        const unsigned short cachePos = lookup->cachePos;
        const float glyphScale = adjustment.Scale * 0.01f;
        if (ch == 0x20)
        {
            const float advance = glyphScale * static_cast<float>(lookup->data.Advance);
            penX += static_cast<signed char>(static_cast<long>(advance < 0.0f ? advance - 0.5f : advance + 0.5f));
            continue;
        }
        const StreamingFontCacheBufferData& buffer = lookup->data.BigChar
            ? static_cast<const StreamingFontCacheBufferData&>(g_StreamingFontCacheBufferData2)
            : static_cast<const StreamingFontCacheBufferData&>(g_StreamingFontCacheBufferData);
        const float u = static_cast<float>(cachePos % buffer.width) / static_cast<float>(buffer.width);
        const float v = static_cast<float>(cachePos / buffer.width) / static_cast<float>(buffer.height);
        const float renderScale = scale * glyphScale;
        const float left = (penX - anchorX) * scale + lookup->data.OffsetX * renderScale + anchorX - 0.5f;
        const float top = (adjustment.Baseline + lookup->data.OffsetY) * renderScale + (penY - anchorY) * scale + anchorY - 0.5f;
        const float glyphWidth = lookup->data.Width;
        const float right = glyphWidth * renderScale + left;
        const float glyphHeight = lookup->data.Height;
        const float bottom = glyphHeight * renderScale + top;
        const float advance = glyphScale * static_cast<float>(lookup->data.Advance);
        penX += static_cast<signed char>(static_cast<long>(advance < 0.0f ? advance - 0.5f : advance + 0.5f));
        const float uRight = glyphWidth * (1.0f / 1024.0f) + u;
        const float vBottom = glyphHeight * (1.0f / 512.0f) + v;
        if (mask)
        {
            const float maskLeft = (left * clipScaleX - 1.0f) * AlphaMaskUVMul.X + AlphaMaskUVAdd.X;
            const float maskTop = (1.0f - top * clipScaleY) * AlphaMaskUVMul.Y + AlphaMaskUVAdd.Y;
            const float maskRight = (right * clipScaleX - 1.0f) * AlphaMaskUVMul.X + AlphaMaskUVAdd.X;
            const float maskBottom = (1.0f - bottom * clipScaleY) * AlphaMaskUVMul.Y + AlphaMaskUVAdd.Y;
            *nextMask++ = CFontVertexMask(C4DVector(left, bottom, position.Z, 1.0f), packedColour, u, vBottom, maskLeft, maskBottom);
            *nextMask++ = CFontVertexMask(C4DVector(left, top, position.Z, 1.0f), packedColour, u, v, maskLeft, maskTop);
            *nextMask++ = CFontVertexMask(C4DVector(right, top, position.Z, 1.0f), packedColour, uRight, v, maskRight, maskTop);
            *nextMask++ = CFontVertexMask(C4DVector(right, bottom, position.Z, 1.0f), packedColour, uRight, vBottom, maskRight, maskBottom);
        }
        else
        {
            *next++ = CFontVertex(C4DVector(left, bottom, position.Z, 1.0f), packedColour, u, vBottom);
            *next++ = CFontVertex(C4DVector(left, top, position.Z, 1.0f), packedColour, u, v);
            *next++ = CFontVertex(C4DVector(right, top, position.Z, 1.0f), packedColour, uRight, v);
            *next++ = CFontVertex(C4DVector(right, bottom, position.Z, 1.0f), packedColour, uRight, vBottom);
        }
        numVertices += 4;
        if (numVertices > 252)
        {
            render->DrawAsIndexedTris(vertices, stride, numVertices, indices, numVertices / 2);
            nextMask = vertices;
            next = reinterpret_cast<CFontVertex*>(vertices);
            numVertices = 0;
        }
    }
    if (numVertices > 0)
        render->DrawAsIndexedTris(vertices, stride, numVertices, indices, numVertices / 2);
    AttachTexture(render, 0, 0);
    AttachTexture(render, 1, 0);
}

namespace {

// Retail 0x009A3640 (Ego_r ?InitializeStreamingFontTextureCache@?A0xfe46a155@@YIXPAVCStreamingFontDataBank@@@Z):
// once, allocate the glyph pixel cache and create the 1024x512 font texture cache (A8, else A4R4G4B4, else the
// display's best 32-bit alpha format) with its alpha-to-texel lookup table, then read the streaming bank's
// glyph-data header, chunk index and adjustments.
void FABLE_FASTCALL InitializeStreamingFontTextureCache(CStreamingFontDataBank* bank)
{
    if (textureCacheMem)
        return;
    textureCacheSize = 0x80000;
    textureCacheMem = static_cast<unsigned char*>(CMemoryAllocator::Allocate(0x80000, 0));
    CPixelFormat format(D3DFMT_A8);
    GFontTextureBytesPerPixel = 1;
    GFontPixelLookupTable.resize(256);
    for (int i = 0; i < 256; ++i)
        GFontPixelLookupTable[i] = static_cast<unsigned char>(i);
    if (!GFontTextureCache.Initialise(C2DExtentsI(1024, 512), 1, format, 0, static_cast<ESurfacePool>(1), false, true))
    {
        CPixelFormat format16(D3DFMT_A4R4G4B4);
        GFontTextureBytesPerPixel = 2;
        GFontPixelLookupTable.resize(512);
        unsigned short* lookup16 = reinterpret_cast<unsigned short*>(GFontPixelLookupTable.GetAsCArray());
        for (int i = 0; i < 256; ++i)
            lookup16[i] = static_cast<unsigned short>((((i * 16) / 256) << 12) | 0x0FFF);
        if (!GFontTextureCache.Initialise(C2DExtentsI(1024, 512), 1, format16, 0, static_cast<ESurfacePool>(1), false, false))
        {
            CPixelFormat best;
            GFGetSystemManager()->PDisplayManager.Data->GetBestAlphaTextureFormat(32, best);
            GFontTextureBytesPerPixel = 4;
            GFontPixelLookupTable.resize(1024);
            unsigned long* lookup32 = reinterpret_cast<unsigned long*>(GFontPixelLookupTable.GetAsCArray());
            for (int i = 0; i < 256; ++i)
                lookup32[i] = ~best.GetAMask() | (i << best.GetAPosShift());
            GFontTextureCache.Initialise(C2DExtentsI(1024, 512), 1, best, 0, static_cast<ESurfacePool>(1), false, false);
        }
    }
    bank->StreamGlyph(&textureCacheNumGlyphs, 0, 4, true);
    bank->StreamGlyph(&textureCacheNumGlyphAdjustments, 4, 4, true);
    bank->StreamGlyph(&textureCacheMemOffset, 8, 4, true);
    bank->StreamGlyph(&glyphTextureCacheGlyphDataSize, 12, 4, true);
    bank->StreamGlyph(&numTextureCacheGlyphDatasPerChunk, 16, 4, true);
    glyphTextureCacheGlyphDataNumIndices = (numTextureCacheGlyphDatasPerChunk + textureCacheNumGlyphs - 1) / numTextureCacheGlyphDatasPerChunk;
    glyphTextureCacheGlyphDataCacheChunk = 0xFFFFFFFF;
    const unsigned long dataOffset = 0x14 + glyphTextureCacheGlyphDataSize;
    const unsigned long indexBytes = (glyphTextureCacheGlyphDataNumIndices + 1) * 4;
    glyphTextureCacheGlyphDataCache = new CStreamingGlyphData[numTextureCacheGlyphDatasPerChunk];
    glyphTextureCacheGlyphData = new unsigned char[glyphTextureCacheGlyphDataSize];
    glyphTextureCacheGlyphDataIndices = new unsigned long[glyphTextureCacheGlyphDataNumIndices + 1];
    glyphTextureCacheGlyphAdjustment = new CStreamingGlyphAdjustment[textureCacheNumGlyphAdjustments];
    bank->ReadData(glyphTextureCacheGlyphData, 0x14, glyphTextureCacheGlyphDataSize);
    bank->ReadData(glyphTextureCacheGlyphDataIndices, dataOffset, indexBytes);
    bank->ReadData(glyphTextureCacheGlyphAdjustment, dataOffset + indexBytes, textureCacheNumGlyphAdjustments * 2);
}

} // namespace

// Retail 0x009A3C20: take a font slot (shared glyph cache entries are tagged with it), make sure the shared
// caches exist, then read the font record: name, height, weight, italics, maximum height, the offsets of its
// 1024 glyph banks and the banks themselves. Every glyph present sets its character's bit in GlyphFlags.
CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank* bank, unsigned long index)
{
    data_bank = bank;
    for (fontIndex = 0; fontIndex < 8; ++fontIndex)
        if (streamingFontBankList[fontIndex] == this)
            break;
    if (fontIndex == 8)
    {
        for (fontIndex = 0; fontIndex < 8; ++fontIndex)
            if (!streamingFontBankList[fontIndex])
                break;
        streamingFontBankList[fontIndex] = this;
    }
    InitializeStreamingFontTextureCache(data_bank);
    InitializeStreamingFontCacheEntries(g_StreamingFontCacheBufferData);
    InitializeStreamingFontCacheEntries(g_StreamingFontCacheBufferData2);
    memset(GlyphBanks, 0, sizeof(GlyphBanks));
    memset(GlyphFlags, 0, sizeof(GlyphFlags));
    const unsigned long size = data_bank->GetEntryDataSize(index);
    CArray<char> record(size);
    data_bank->ReadEntryData(index, 0, record.GetAsCArray(), size);
    CMemoryDataInputStream stream(record.GetAsCArray(), size);
    glyphBankCache = new GlyphBank;
    glyphBankCacheIndex = 0xFFFFFFFF;
    Name = stream.ReadNullTerminatedString();
    FontHeight = stream.ReadValue<long>();
    Weight = stream.ReadValue<long>();
    Italics = stream.ReadEBOOL();
    MaxHeight = stream.ReadULONG();
    unsigned long bankOffsets[1025];
    stream.Read(bankOffsets, sizeof(bankOffsets));
    GlyphBankMemory = new unsigned char[bankOffsets[1024]];
    stream.Read(GlyphBankMemory, bankOffsets[1024]);
    for (unsigned long b = 0; b < 1024; ++b)
    {
        const unsigned long length = bankOffsets[b + 1] - bankOffsets[b];
        if (!length)
            continue;
        StoredGlyphBank* stored = reinterpret_cast<StoredGlyphBank*>(static_cast<unsigned char*>(GlyphBankMemory) + bankOffsets[b]);
        GlyphBanks[b] = stored;
        GlyphBank* glyphs = reinterpret_cast<GlyphBank*>(stored->data);
        if (stored->compressed)
        {
            unsigned long unpacked = sizeof(GlyphBank);
            lzo1x_decompress_safe(stored->data, length - 2, reinterpret_cast<unsigned char*>(glyphBankCache), &unpacked, 0);
            glyphs = glyphBankCache;
            glyphBankCacheIndex = b;
        }
        for (unsigned long g = 0; g < glyphs->NoGlyphs; ++g)
        {
            if (!glyphs->Glyphs[g].GlyphIndex)
                continue;
            const unsigned short ch = static_cast<unsigned short>((b << 6) + g + glyphs->FirstGlyph);
            GlyphFlags[ch >> 5] |= 1 << (ch & 31);
        }
    }
    PTexture = new CTexture;
}

// Retail 0x009A1090: give back the font slot, drop this font's glyph lookups (the cache slots they point at stay)
// and free the glyph banks, bank cache and texture.
CStreamingFontBank::~CStreamingFontBank()
{
    if (streamingFontBankList[fontIndex] == this)
        streamingFontBankList[fontIndex] = 0;
    for (unsigned long bucket = 0; bucket < 256; ++bucket)
    {
        for (unsigned long way = 0; way < 8; ++way)
        {
            CStreamingGlyphLookup& entry = glyphLookup[bucket][way];
            if (entry.font == fontIndex)
            {
                entry.ch = 0;
                entry.font = 0;
            }
        }
    }
    delete[] GlyphBankMemory;
    delete glyphBankCache;
    delete PTexture;
    PTexture = 0;
}

// Retail 0x009A1660 (Ego_r ?UninitializeStreamingFontTextureCache@@YIXXZ): free the shared glyph data and the
// glyph pixel cache.
void FABLE_FASTCALL UninitializeStreamingFontTextureCache()
{
    if (!textureCacheMem)
        return;
    delete[] glyphTextureCacheGlyphAdjustment;
    glyphTextureCacheGlyphAdjustment = 0;
    delete[] glyphTextureCacheGlyphDataIndices;
    glyphTextureCacheGlyphDataIndices = 0;
    delete[] glyphTextureCacheGlyphData;
    glyphTextureCacheGlyphData = 0;
    delete[] glyphTextureCacheGlyphDataCache;
    glyphTextureCacheGlyphDataCache = 0;
    CMemoryAllocator::Deallocate(textureCacheMem);
    textureCacheMem = 0;
}

// Keep the LRU helpers referenced until their callers (texture cache initialisation, PrepareToRender, Render)
// land in this unit.
void (FABLE_FASTCALL* const StreamingFontCacheHelpers_009a0f00[])(StreamingFontCacheBufferData&) = {&InitializeStreamingFontCacheEntries};
void (FABLE_FASTCALL* const StreamingFontCacheUpdate_009a0f90)(StreamingFontCacheBufferData&, unsigned short) = &UpdateUseStreamingFontCacheEntry;
unsigned short (FABLE_FASTCALL* const StreamingFontCacheInsert_009a1020)(StreamingFontCacheBufferData&) = &InsertNewStreamingFontCacheEntry;
void (FABLE_FASTCALL* const StreamingFontTextureCacheInit_009a3640)(CStreamingFontDataBank*) = &InitializeStreamingFontTextureCache;
void (FABLE_FASTCALL* const StreamingFontTextureCacheUpdate_009a23e0)() = &GFUpdateFontTextureCache;
unsigned long (FABLE_FASTCALL* const StreamingFontReadText_009a3460)(const CWideString&, CArray<unsigned short>&) = &ReadTextFileIntoArray;
unsigned long (FABLE_FASTCALL* const StreamingFontReadText_009a3610)(const CCharString&, CArray<unsigned short>&) = &ReadTextFileIntoArray;
