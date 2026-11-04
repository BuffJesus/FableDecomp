// lib_display_manager.cpp, retail 0x009BED10 (Ego_r
// ?GetCurrentWindowedSettings@CDisplayManager@@AAEXPAUHWND__@@AAVC2DExtentsI@@AAJ@Z):
// a windowed mode takes the window's outer size and the desktop's colour depth.
#include "fable_display_viewport.h"

extern "C" __declspec(dllimport) int __stdcall GetWindowRect(HWND__* window, tagRECT* rect);

void CDisplayManager::GetCurrentWindowedSettings(HWND__* window, C2DExtentsI& dimensions, long& bit_depth)
{
    D3DDISPLAYMODE mode;
    D3D->GetAdapterDisplayMode(D3DDeviceCaps.AdapterOrdinal, &mode);
    tagRECT rect;
    GetWindowRect(window, &rect);
    dimensions.X = rect.right - rect.left;
    dimensions.Y = rect.bottom - rect.top;
    CPixelFormat format(mode.Format);
    bit_depth = format.GetColourDepth();
}
