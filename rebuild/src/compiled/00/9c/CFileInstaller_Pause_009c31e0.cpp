// Retail 009C31E0; first source exact, including unsigned counter wrap and unlocked alertable wait.
#include "fable_file_installer.h"
#include "fable_critical_section_lock.h"
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void*, unsigned long, int);

void CFileInstaller::Pause()
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection), false);
    ++Paused;
    if (Running && Paused <= 1 && !DirtyDisc) {
        lock.Release();
        SetEvent(Event);
        WaitForSingleObjectEx(PauseStartEvent, 0xffffffffUL, 1);
    }
}
