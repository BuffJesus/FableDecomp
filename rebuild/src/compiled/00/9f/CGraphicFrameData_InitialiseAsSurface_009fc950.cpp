#include "fable_graphic_frame_data.h"
#include "fable_surface_types.h"

template<> inline CCountedPointer<CTexture>& CCountedPointer<CTexture>::operator=(const CCountedPointer& other)
{
    CTexture* data=other.Data;
    CCPPointerInfo* info=other.Info;
    if(Info!=info){
        Release();
        Data=data;Info=info;
        if(Info)++Info->RefCount;
    }
    return *this;
}

bool CGraphicFrameData::InitialiseAsSurface(const CCountedPointer<CTexture>& texture,
    long width, long height, bool alpha, bool booleanAlpha)
{
    UsesSurface=true;
    PTexture=texture;
    Width=width;Pitch=width;Height=height;
    Alpha=alpha;BooleanAlpha=booleanAlpha;SectionOfTPage=false;IsInitialised=true;
    TLX=TLY=0;
    // Both original retail and donor compare against X on both calls.
    ScreenGraphic=(Width!=texture->GetWidth() || Height!=texture->GetWidth());
    return true;
}
