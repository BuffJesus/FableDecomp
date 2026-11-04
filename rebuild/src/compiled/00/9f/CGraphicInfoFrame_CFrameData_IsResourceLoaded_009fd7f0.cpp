#include "fable_graphic_info.h"
bool CGraphicInfoFrame::CFrameData::IsResourceLoaded()const{if(!FileData.IsNull() && !FileData->IsFinished())return false;return true;}
