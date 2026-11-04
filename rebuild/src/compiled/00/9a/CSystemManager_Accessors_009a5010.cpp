// lib_system_manager.cpp, retail 0x009A5010 / 0x009A5210 / 0x009A5240 / 0x009A5250 /
// 0x009A52D0 / 0x009A5460 (Ego_r CSystemManager names): small accessors and
// the external-window attach/detach pair. Initialise 0x009A6610 expands its own
// copy of InitFromExternalApplicationWindow; this is the out-of-line one.
#include "fable_system_manager_state.h"

extern "C" __declspec(dllimport) int __stdcall DestroyWindow(HWND__* window);

// Retail 009A5010.
long CSystemManager::GetSize() const
{
    return sizeof(CSystemManager);
}

// Retail 009A5210.
void CSystemManager::UninitWindow()
{
    if (WindowValid == true)
    {
        DestroyWindow(static_cast<HWND__*>(ApplicationWinHandle));
        ApplicationWinHandle = 0;
        WindowValid = false;
    }
}

// Retail 009A5240.
bool CSystemManager::InitFromExternalApplicationWindow(HWND__* window)
{
    ApplicationWinHandle = window;
    return true;
}

// Retail 009A5250.
void CSystemManager::UninitFromExternalWindow()
{
    ApplicationWinHandle = 0;
}

// Retail 009A52D0.
CDebugManager* CSystemManager::GetDebugManager()
{
    return DebugManager;
}

// Retail 009A5460.
CCharString CSystemManager::GetApplicationName() const
{
    return ApplicationName;
}
