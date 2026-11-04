#define FABLE_TEXTURE_OUT_OF_LINE_GET_NO_LEVELS
#define FABLE_MANAGED_TEXTURE_INLINE_MEMBERS
#define FABLE_GRAPHIC_FRAME_INLINE_UNINITIALISE
#include "engine/CTextureManager.h"
#include "fable_surface_types.h"
#include "fable_graphic_frame.h"
#include "fable_managed_texture.h"
template<> inline CCountedPointer<CTexture>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data),Info(other.Info) {if(Info)++Info->RefCount;}
inline bool CManagedTexture::Release(){
    if(Data.IsNull())return true;
    bool released=false;
    if(!Data.IsNull() && Data->IVRefCount==2){Data->ReleaseTexture();released=true;}
    Data.Release();Data.data_=0;return released;
}
inline CManagedTexture::~CManagedTexture(){if(!Data.IsNull() && Data->IVRefCount==2)Release();}
inline const CManagedTexture& CManagedTexture::operator=(const CManagedTexture& other){
    if(Data.data_!=other.Data.data_){Release();Data=other.Data;}return *this;
}
inline void CGraphicFrame::UnInitialise(){
    CGraphicFrameData* data=FrameData;
    if(data){if(--data->ReferenceCount==0){data->~CGraphicFrameData();operator delete(data);}FrameData=0;}
}
bool CTextureManager::ReduceMipmapLevel(CGraphicFrame& frame,CManagedTexture& managed,unsigned long reduction){
    CTexture* original=frame.FrameData->PTexture.Data;
    unsigned long levels=original->GetNoLevels()-reduction;
    long width=frame.FrameData->Width,height=frame.FrameData->Height;
    for(unsigned long i=0;i<reduction;++i){width/=2;height/=2;}
    managed.Data.data_->Locked=true;
    CManagedTexture replacement;
    if(!AllocateTexture(replacement,width,height,levels,original->GetPixelFormat(),managed.Data.data_->Owner)){
        managed.Data.data_->Locked=false;return false;
    }
    CCountedPointer<CTexture> texture=replacement.Data.data_->Texture;
    C2DBoxI rectangle;rectangle.TLX=rectangle.TLY=0;rectangle.BRX=width;rectangle.BRY=height;
    for(unsigned long level=0;level<levels;++level){
        CSurface destination=texture->GetSurfaceLevel(level);
        CSurface source=original->GetSurfaceLevel(level+reduction);
        destination.CopyFromSurface(&rectangle,source,&rectangle,1);
        rectangle.BRX=(long)rectangle.BRX/2;rectangle.BRY=(long)rectangle.BRY/2;
    }
    CGraphicFrame previous=frame;if(previous.FrameData)++previous.FrameData->ReferenceCount;
    frame.UnInitialise();
    frame.InitialiseAsSurface(texture,width,height,previous.FrameData->Alpha,previous.FrameData->BooleanAlpha);
    managed.Data.data_->Owner=0;managed.Data.data_->Locked=false;
    managed=replacement;return true;
}
