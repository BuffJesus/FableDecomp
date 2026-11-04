#include "fable_graphic_info.h"
void CGraphicInfoFrame::CFrameData::ReleaseTexture(){if(FileData.IsNull())GraphicInfoFrame->GraphicInfo->UnloadFrame(GraphicInfoFrame->FrameNum);}
