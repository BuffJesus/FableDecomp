#include "fable_graphic_info.h"
CGraphicInfoFrame::CFrameData::~CFrameData(){
    ManagedTexture.Release();
    Frame.UnInitialise();
    CurrentMipmapLevel=-1;
    TouchedMipmapLevel=127;
}
