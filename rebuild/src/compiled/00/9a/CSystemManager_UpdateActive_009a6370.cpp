// CSystemManager::UpdateActive, retail 0x009A6370
// (FableWin ?UpdateActive@CSystemManager@@QAEXXZ): pump window messages and
// track activation/input focus; while the application is inactive and asked
// to wait, keep pumping (sleeping while the display device is lost).
// SetAsActive and SetHasInputFocus are inlined here, as in retail.
#include "fable_system_manager.h"

extern "C" __declspec(dllimport) void* FABLE_STDCALL GetForegroundWindow();
extern "C" __declspec(dllimport) int FABLE_STDCALL WaitMessage();
extern "C" __declspec(dllimport) void FABLE_STDCALL Sleep(unsigned long milliseconds);

// FableWin ?GFRecalibrateFastSubFrameTimer@@YIXXZ (retail 0x009E24B0).
void FABLE_FASTCALL GFRecalibrateFastSubFrameTimer();

inline void CSystemManager::SetAsActive(bool active)
{
    if (active && !ApplicationActive && EverBeenActive == true)
        Restore = true;
    ApplicationActive = active;
    EverBeenActive = true;
}

inline void CSystemManager::SetHasInputFocus(bool hasFocus)
{
    ApplicationHasInputFocus = hasFocus;
    if (PInputManager.Get())
        PInputManager->SetHasInputFocus(hasFocus);
}

void CSystemManager::UpdateActive()
{
    GFRecalibrateFastSubFrameTimer();
    if (IsAttachedToExternalWindow())
        return;

    while (!WindowsQuit)
    {
        if (!ApplicationActive && !WindowsQuit && WaitWhileInactive == true)
            WaitMessage();

        ProcessWindowMessages();

        if (GetForegroundWindow() != ApplicationWinHandle)
        {
            SetAsActive(false);
            SetHasInputFocus(false);
        }
        else
        {
            SetAsActive(true);
            SetHasInputFocus(true);
        }

        if (!PDisplayManager->ProcessLostDevice())
            Sleep(10);
        else if (ApplicationActive || WindowsQuit || !WaitWhileInactive)
            break;
    }
}