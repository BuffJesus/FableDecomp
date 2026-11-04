#include "fable_graphic_info.h"
void CGraphicInfoFrame::CFrameData::OnResourceLoaded(){if(!FileData.IsNull()){if(!UploadFrame(false) && !Frame.FrameData){CIVCountedPointer<CFrameData>& data=GraphicInfoFrame->Data;data.Release();data.data_=0;}}}
