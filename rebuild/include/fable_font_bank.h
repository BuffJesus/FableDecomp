#pragma once

// Hand-owned retail font interfaces. Keep these out of donor-header regeneration.



// Curated retail interface; field names/offsets from Ego_r and 009E2BA0/009E2C10.
#include "fable_string.h"
#include "fable_base_class.h"
#include "fable_extents_f.h"

class CFontManager;
class C3DVector;
class CRGBColour;
class CTexture;

class CFontBank : public CBaseClassNonCopyable
{
public:
    // Native base/name construction; height and manager are left for derived owners.
    CFontBank() {}
    virtual ~CFontBank() {}
    virtual C2DExtentsF GetStringExtents(const wchar_t* text, bool flag) const = 0;
    virtual float GetFontMaxHeight() const = 0;
    virtual void PrepareToRender(bool virtualPixels) const = 0;
    // FableWin ?Preload@CFontBank@@UBE_NABVCWideString@@_N@Z (+0x10).
    virtual bool Preload(const CWideString& text, bool wait) const;
    virtual void Render(const CWideString& text,const C3DVector& position,float scale,
        const CRGBColour& colour,unsigned long flags,const CTexture* texture) const = 0;

    C2DExtentsF GetVStringExtents(const CWideString& text) const; // retail00437210, aggregate return
    float GetVStringWidth(const CWideString& text) const; // retail 0043E4D0

    unsigned long FontHeight;
    CCharString Name; // owned, +0x08; released before the noncopyable base
    CFontManager* PManager;
};
FABLE_STATIC_ASSERT(sizeof(CFontBank) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CFontBank, PManager) == 0x0C);

FABLE_STATIC_ASSERT(offsetof(CFontBank, FontHeight) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CFontBank, Name) == 0x08);
