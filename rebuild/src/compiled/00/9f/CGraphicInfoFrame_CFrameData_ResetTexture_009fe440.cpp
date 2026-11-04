#include "fable_graphic_info.h"
void CGraphicInfoFrame::CFrameData::ResetTexture() {
    ManagedTexture.Release();
    Frame.UnInitialise();
    CurrentMipmapLevel=-1;
    TouchedMipmapLevel=127;
}
