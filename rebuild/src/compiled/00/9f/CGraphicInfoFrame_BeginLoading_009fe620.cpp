#include "fable_graphic_info.h"
void CGraphicInfoFrame::BeginLoading(unsigned long first,unsigned long second) const {
    const CGraphicInfoFrame* frame=this;
    while(frame->FrameNum>0 && frame->GraphicInfo->Header.IsFlatAnimatedGraphic)
        frame=frame->GraphicInfo->Frames;
    if(frame->Data.IsNull()) {
        CFrameData* fresh=new CFrameData(frame);
        frame->Data.Release();
        frame->Data.data_=fresh;
    }
    frame->Data->BeginLoading(first,second);
}
