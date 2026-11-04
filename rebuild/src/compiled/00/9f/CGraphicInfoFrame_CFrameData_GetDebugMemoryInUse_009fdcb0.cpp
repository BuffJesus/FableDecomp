#define FABLE_TEXTURE_OUT_OF_LINE_GET_NO_LEVELS
#include "fable_graphic_info.h"
#include "fable_surface_types.h"
template<class T> static const T& nativeMin(const T& a,const T& b) {return a<b?a:b;}
unsigned long CGraphicInfoFrame::CFrameData::GetDebugMemoryInUse() const {
    if(TouchedMipmapLevel==127 || !Frame.FrameData)return 0;
    unsigned long size=sizeof(*this)+Frame.Sizeof();
    if(!LoadingManagedTexture.Data.IsNull())size+=LoadingManagedTexture.Sizeof();
    long levels=nativeMin<long>(GraphicInfoFrame->SmallMipmapLevel,TouchedMipmapLevel);
    levels=nativeMin<long>(levels,Frame.FrameData->PTexture->GetNoLevels());
    long bytes=Frame.FrameData->PTexture->GetSurfaceLevel(0).GetByteLength();
    for(long i=0;i<levels;++i){size-=bytes;bytes/=4;}
    return size;
}
