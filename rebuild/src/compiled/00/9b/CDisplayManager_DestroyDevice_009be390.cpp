// Retail 009BE390; FableWin private CDisplayManager::DestroyDevice.
#include "fable_surface.h"
void CDisplayManager::DestroyDevice()
{
    GetBackBuffer().Uninitialise();
    DepthBuffer.Uninitialise();
    if (D3DDevice) D3DDevice->Release();
}
