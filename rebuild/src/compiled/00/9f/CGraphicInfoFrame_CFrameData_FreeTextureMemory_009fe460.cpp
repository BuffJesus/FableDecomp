#include "fable_graphic_info.h"
#include "engine/CTextureManager.h"
bool CGraphicInfoFrame::CFrameData::FreeTextureMemory(bool discard){
    if(Frame.FrameData && GraphicInfoFrame->GraphicInfo->Bank->TexturePointersValidForFrames<=ManagedTexture.Data->GetUnusedFrames()){
        unsigned long unused=GetUnusedFrames();
        CGraphicInfo* info=GraphicInfoFrame->GraphicInfo;
        CGraphicDataBank* bank=info->Bank;
        if(unused<=bank->ReduceSizeFrameDelay)return false;
        unsigned long level=(int)CurrentMipmapLevel+1;
        if(!info->Header.IsFlatAnimatedGraphic && level<=(unsigned long)(int)GraphicInfoFrame->SmallMipmapLevel){
            bank->TextureManager->ReduceMipmapLevel(Frame,ManagedTexture,1);
            CurrentMipmapLevel=(signed char)level;
            SetResourceSize(Sizeof());
            return true;
        }
        if(discard){ResetTexture();GraphicInfoFrame->ReleaseData();return true;}
    }
    unsigned long frameSize=Frame.FrameData?Frame.FrameData->Sizeof()+4:4;
    unsigned long size=68+frameSize;
    if(!LoadingManagedTexture.Data.IsNull())size+=LoadingManagedTexture.Sizeof();
    SetResourceSize(size);
    return false;
}
