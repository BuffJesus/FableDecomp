#pragma once

// Hand-owned retail font interfaces. Keep these out of donor-header regeneration.

#ifndef FABLE_ENGINE_CSTATICFONTBANK_H
#define FABLE_ENGINE_CSTATICFONTBANK_H
// Curated from Ego_r; constructor ABI and allocation size confirmed in retail AcquireFont.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CStaticFontBank = 0x2034.
// Layout and explicitly gated members only; concrete glyph construction/destruction remain open.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_font_bank.h"

class CTexture;

class CFontDataBank;

// Static glyph records; the streaming bank's similarly named record has a different layout.
class CDataInputStream;
struct CGlyph {
    void Read(CDataInputStream& stream);
    float Left, Top, Right, Bottom;
    short Offset, Width, Advance;
    unsigned char _pad_0x16[2];
};
struct CStaticFontGlyphBank {
    unsigned short FirstGlyph, NoGlyphs;
    CGlyph* Glyphs;
};
FABLE_STATIC_ASSERT(sizeof(CGlyph) == 24);
FABLE_STATIC_ASSERT(offsetof(CGlyph, Offset) == 16);
FABLE_STATIC_ASSERT(offsetof(CGlyph, Advance) == 20);
FABLE_STATIC_ASSERT(sizeof(CStaticFontGlyphBank) == 8);
#pragma pack(push, 1)
class CStaticFontBank : public CFontBank {
public:
    virtual ~CStaticFontBank();
    virtual C2DExtentsF GetStringExtents(const wchar_t*, bool) const;
    virtual float GetFontMaxHeight() const;
    virtual void PrepareToRender(bool) const;
    virtual void Render(const CWideString&, const C3DVector&, float, const CRGBColour&, unsigned long, const CTexture*) const;
    CStaticFontBank(CFontDataBank* bank, unsigned long index);
    unsigned long MaxHeight;                                 // +0x10
    long          Weight;                                    // +0x14
    bool          Italics;                                   // +0x18
    unsigned char _pad_0x19[0x3];                            // +0x19
    long          TextureSize_X;                             // +0x1c C2DExtentsI::X
    long          TextureSize_Y;                             // +0x20 C2DExtentsI::Y
    long          MinChar;                                   // +0x24
    long          MaxChar;                                   // +0x28
    long          TotalNoGlyphs;                             // +0x2c
    CStaticFontGlyphBank GlyphBanks[1024];                        // +0x30 GlyphBank[1024]
    CTexture*     PTexture;                                  // +0x2030
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CStaticFontBank) == 0x2034);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, FontHeight) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, MaxHeight) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, Weight) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, Italics) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, TextureSize_X) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, TextureSize_Y) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, MinChar) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, MaxChar) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, TotalNoGlyphs) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, GlyphBanks) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CStaticFontBank, PTexture) == 0x2030);

#endif // FABLE_ENGINE_CSTATICFONTBANK_H

#ifndef FABLE_ENGINE_CSTREAMINGFONTBANK_H
#define FABLE_ENGINE_CSTREAMINGFONTBANK_H
// Curated from Ego_r; constructor ABI and allocation size confirmed in retail AcquireFont.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CStreamingFontBank = 0x3034.
// Layout and explicitly gated members only; concrete glyph construction/destruction remain open.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CStreamingFontDataBank;
class CTexture;
struct GlyphBank;

class CStreamingFontDataBank;
class C2DVector;

#pragma pack(push, 1)
class CStreamingFontBank : public CFontBank {
public:
    virtual ~CStreamingFontBank();
    virtual C2DExtentsF GetStringExtents(const wchar_t*, bool) const;
    virtual float GetFontMaxHeight() const;
    virtual void PrepareToRender(bool) const;
    virtual void Render(const CWideString&, const C3DVector&, float, const CRGBColour&, unsigned long, const CTexture*) const;
    virtual bool Preload(const CWideString&, bool) const;
    CStreamingFontBank(CStreamingFontDataBank* bank, unsigned long index);
    // Ego_r statics; retail SetAlphaMaskUVs 0x009A0EC0 / SetUseFixedFunction 0x009A0EF0 write
    // AlphaMaskUVMul 0x013CA2F8, AlphaMaskUVAdd 0x013CA300 and UseFixedFunction 0x013C5A7C.
    static void FABLE_FASTCALL SetAlphaMaskUVs(const C2DVector& mul, const C2DVector& add);
    static void FABLE_FASTCALL SetUseFixedFunction(bool useFixedFunction);
    static C2DVector AlphaMaskUVMul;
    static C2DVector AlphaMaskUVAdd;
    static bool UseFixedFunction;
protected:
    // Ego_r ?FindGlyph@CStreamingFontBank@@IBEPBVCStreamingGlyph@@G@Z, retail 0x009A4160.
    const class CStreamingGlyph* FindGlyph(unsigned short ch) const;
    // Ego_r ?GlyphLookup@CStreamingFontBank@@IBEPAUCStreamingGlyphLookup@@G@Z, retail 0x009A1140.
    struct CStreamingGlyphLookup* GlyphLookup(unsigned short ch) const;
public:
    CStreamingFontDataBank* data_bank;                       // +0x10
    // FindGlyph (const) decompresses into this one-bank cache and records its index.
    mutable GlyphBank*      glyphBankCache;                  // +0x14
    mutable unsigned long   glyphBankCacheIndex;             // +0x18
    unsigned long           MaxHeight;                       // +0x1c
    long                    Weight;                          // +0x20
    bool                    Italics;                         // +0x24
    unsigned char           _pad_0x25[0x3];                  // +0x25
    void*                   GlyphBankMemory;                 // +0x28
    void*                   GlyphBanks[1024];                // +0x2c
    unsigned long           GlyphFlags[2048];                // +0x102c
    unsigned long           fontIndex;                       // +0x302c
    CTexture*               PTexture;                        // +0x3030
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CStreamingFontBank) == 0x3034);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, FontHeight) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, data_bank) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, glyphBankCache) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, glyphBankCacheIndex) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, MaxHeight) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, Weight) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, Italics) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, GlyphBankMemory) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, GlyphBanks) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, GlyphFlags) == 0x102c);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, fontIndex) == 0x302c);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontBank, PTexture) == 0x3030);

#endif // FABLE_ENGINE_CSTREAMINGFONTBANK_H
