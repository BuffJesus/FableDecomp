#include "fable_system_manager_state.h"
#include "fable_window_message.h"

extern "C" __declspec(dllimport) void FABLE_STDCALL EnterCriticalSection(void*);
extern "C" __declspec(dllimport) void FABLE_STDCALL LeaveCriticalSection(void*);
extern "C" __declspec(dllimport) int FABLE_STDCALL PeekMessageW(
    tagMSG*, HWND__*, unsigned int, unsigned int, unsigned int);
extern "C" __declspec(dllimport) int FABLE_STDCALL TranslateMessage(const tagMSG*);
extern "C" __declspec(dllimport) long FABLE_STDCALL DispatchMessageW(const tagMSG*);

void CSystemManager::ProcessWindowMessages()
{
    tagMSG message;
    EnterCriticalSection(CriticalSection);
    while (PeekMessageW(&message, static_cast<HWND__*>(ApplicationWinHandle), 0, 0, 1))
    {
        if (!MessageFilter || !MessageFilter(&message))
        {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }
    LeaveCriticalSection(CriticalSection);
}
