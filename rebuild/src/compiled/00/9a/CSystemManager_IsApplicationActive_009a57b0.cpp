// CSystemManager::IsApplicationActive, retail 0x009A57B0
// (FableWin ?IsApplicationActive@CSystemManager@@QAE_NXZ): the application is
// active while its window is the foreground window; record that (SetAsActive,
// inlined here as in CSystemManager_Initialise_009a6610.cpp) and report it.
// FableWin's debug-only IsValid assert is compiled out.
#include "fable_system_manager.h"

extern "C" __declspec(dllimport) void* FABLE_STDCALL GetForegroundWindow();

inline void CSystemManager::SetAsActive(bool active)
{
    if (active && !ApplicationActive && EverBeenActive == true)
        Restore = true;
    ApplicationActive = active;
    EverBeenActive = true;
}

bool CSystemManager::IsApplicationActive()
{
    void* window = ApplicationWinHandle;
    bool active = window == GetForegroundWindow();
    SetAsActive(active);
    return ApplicationActive;
}