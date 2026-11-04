#include "fable_graphic_info.h"
unsigned long CGraphicInfoFrame::CFrameData::Sizeof() const {
    unsigned long frameSize=Frame.FrameData?Frame.FrameData->Sizeof()+4:4;
    unsigned long size=68+frameSize;
    if(!LoadingManagedTexture.Data.IsNull())size+=LoadingManagedTexture.Sizeof();
    return size;
}
