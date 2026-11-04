// Retail 009BE490; FableWin public CDisplayManager method.
#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
struct tagPOINT { long x,y; };
extern "C" {
__declspec(dllimport) int FABLE_STDCALL GetSystemMetrics(int);
__declspec(dllimport) int FABLE_STDCALL ClientToScreen(HWND__*,tagPOINT*);
__declspec(dllimport) int FABLE_STDCALL MoveWindow(HWND__*,int,int,int,int,int);
}
void CDisplayManager::ResizeWindowToBackBufferSize()
{
    if (!ExclusiveMode && !SystemManager->IsAttachedToExternalWindow()) {
        int borderX=GetSystemMetrics(32)*2;
        int borderY=GetSystemMetrics(33)*2+GetSystemMetrics(4);
        tagPOINT origin;
        origin.x=0;
        origin.y=0;
        ClientToScreen(WindowHandle,&origin);
        MoveWindow(WindowHandle,origin.x,origin.y,
            BackBufferDimensionsStorage_[0]+borderX,
            BackBufferDimensionsStorage_[1]+borderY,1);
    }
}
