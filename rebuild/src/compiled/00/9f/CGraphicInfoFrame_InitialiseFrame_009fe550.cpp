#include "fable_graphic_info.h"

void CGraphicInfoFrame::InitialiseFrame(
    CGraphicFrame& frame, CManagedTexture& managedTexture, signed char mipmapLevel)
{
    if (Data.IsNull()) {
        CFrameData* fresh = new CFrameData(this);
        Data.Release();
        Data.data_ = fresh;
    }
    Data->Frame = frame;
    Data->ManagedTexture = managedTexture;
    managedTexture.Release();
    Data->CurrentMipmapLevel = mipmapLevel;
}
