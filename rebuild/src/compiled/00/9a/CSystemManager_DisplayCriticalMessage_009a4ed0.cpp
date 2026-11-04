// Retail 009A4ED0; FableWin DisplayCriticalMessage(const char*).
#include "fable_system_manager_state.h"

extern "C" __declspec(dllimport) int FABLE_STDCALL MessageBoxW(
    HWND__* window, const wchar_t* message, const wchar_t* caption,
    unsigned int type);

void CSystemManager::DisplayCriticalMessage(const char* message)
{
    CCharString text(message);
    MessageBoxW(0, text.ToWideString().GetAsCharArray(),
                L"Application Error", 0);
}
