#pragma once
#include "rebuild_abi.h"
#include "fable_counted_pointer.h"
class CTexture;
// Original nonpolymorphic packed 27B object and one-byte flag bitfields.
#pragma pack(push,1)
class CGraphicFrameData {
public:
    CGraphicFrameData() : ReferenceCount(1), IsInitialised(false) {}
    __forceinline ~CGraphicFrameData() {if(IsInitialised)Uninitialise();}
    bool InitialiseAsSurface(const CCountedPointer<CTexture>& texture,
        long width, long height, bool alpha, bool booleanAlpha);
    // Donor/original declaration only; the 009FCA40 implementation is unresolved.
    bool InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& texture,
        float x, float y, long width, long height, bool alpha, bool booleanAlpha);
    void Uninitialise();
    unsigned long Sizeof() const;
    unsigned long ReferenceCount;
    void* Data;
    CCountedPointer<CTexture> PTexture;
    unsigned short TLX,TLY,Width,Height,Pitch;
    unsigned char Alpha:1;
    unsigned char BooleanAlpha:1;
    unsigned char UsesSurface:1;
    unsigned char SectionOfTPage:1;
    unsigned char IsInitialised:1;
    unsigned char ScreenGraphic:1;
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CGraphicFrameData)==27);
