// CDisplayManager::SwapScreens (retail 0x009BEEB0).
// FableWin.pdb ?SwapScreens@CDisplayManager@@QAE_NXZ. Shape from the FableWin
// debug body: a lost device only raises DeviceLostFlag; any failure returns false.
#include "fable_display_viewport.h"

bool CDisplayManager::SwapScreens()
{
    ++FrameCounter;
    const HRESULT result = D3DDevice->Present(0, 0, 0, 0);
    if (result == D3DERR_DEVICELOST)
        DeviceLostFlag = true;
    if (result != 0)
        return false;
    VsyncCallbackTimer->OnPostSwapScreens();
    CurrentScreenIndex = (CurrentScreenIndex + 1) % (NoBackBuffers + 1);
    return true;
}

// Retail 0x009BF2D0 (Ego_r ?FlipToGDISurface@CDisplayManager@@QAEXXZ): in
// exclusive mode, keep presenting until the front buffer is screen 0 again
// (SwapScreens inlined).
void CDisplayManager::FlipToGDISurface()
{
    if (ExclusiveMode)
    {
        while (CurrentScreenIndex != 0)
            SwapScreens();
    }
}
