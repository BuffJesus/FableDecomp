#include "fable_graphic_info.h"
CGraphicInfoFrame::CFrameData::CFrameData(const CGraphicInfoFrame* frame)
    : GraphicInfoFrame(frame) {
    CurrentMipmapLevel=-1;
    TouchedMipmapLevel=127;
    TouchedFrame=frame->GraphicInfo->Bank->ResourceList.CurrentFrame-1;
    unsigned long size=sizeof(*this)+Frame.Sizeof();
    if(!LoadingManagedTexture.Data.IsNull())size+=LoadingManagedTexture.Sizeof();
    SetResourceSize(size);
    frame->GraphicInfo->Bank->AddResource(*this);
}
