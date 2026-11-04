#include "fable_graphic_info.h"
template<class T> inline void CCountedPointer<T>::ReleaseData(){Release();Info=0;Data=0;}
bool CGraphicInfoFrame::CFrameData::UploadFrame(bool keepData){
    bool loaded=false;
    if(GraphicInfoFrame->GraphicInfo->Bank->GetEntryDataType(GraphicInfoFrame->GraphicInfo->Index)==4){
        unsigned long level=(int)LoadingMipmapLevel;
        const CGraphicInfoFrame* owner=GraphicInfoFrame;
        CGraphicInfo* info=owner->GraphicInfo;
        CGraphicDataBank* bank=info->Bank;
        CBankFileAsyncData* file=FileData.Get();
        CManagedTexture& loading=LoadingManagedTexture;
        if(bank->LoadVolumeGraphic(loading,*file,owner->GraphicInfo->Header,true,level,bank->GetEntryDataType(info->Index)==2)){ManagedTexture=loading;loading.Release();CurrentMipmapLevel=LoadingMipmapLevel;loaded=true;}
    }else{
        CGraphicFrame frame;
        CGraphicInfo* info=GraphicInfoFrame->GraphicInfo;
        if(info->Bank->LoadGraphicFrame(frame,LoadingManagedTexture,*FileData.Get(),info,true,(int)LoadingMipmapLevel,info->TextureType,keepData)){
            if(!GraphicInfoFrame->GraphicInfo->Header.IsFlatAnimatedGraphic){Frame=frame;ManagedTexture=LoadingManagedTexture;LoadingManagedTexture.Release();CurrentMipmapLevel=LoadingMipmapLevel;}
            loaded=true;
        }
    }
    FileData.ReleaseData();
    unsigned long frameSize=Frame.FrameData?Frame.FrameData->Sizeof()+4:4;
    unsigned long size=68+frameSize;
    if(!LoadingManagedTexture.Data.IsNull())size+=LoadingManagedTexture.Sizeof();
    SetResourceSize(size);
    GraphicInfoFrame->GraphicInfo->Bank->AddResource(*this);
    return loaded;
}
