#include "fable_graphic_info.h"
#include "fable_surface_types.h"
unsigned long CGraphicInfoFrame::CFrameData::GetUnloadableMemory()const{
 if(!FileData.IsNull()){if(FileData->IsFinished())return FileData->Length;}
 else if(Frame.FrameData && Frame.FrameData->PTexture.Get())return Frame.FrameData->PTexture.Get()->GetLevelByteLength(0);
 return 0;
}
