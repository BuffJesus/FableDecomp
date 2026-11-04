// Retail 009C3250; NEAR_MATCH, PDB-volatile pause state and real scoped-lock lifetime.
#include "fable_file_installer.h"
#include "fable_critical_section_lock.h"
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);

void CFileInstaller::Unpause()
{
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection), false);
    if (Paused > 0) {
        --Paused;
        if (Running && Paused == 0) {
            SetEvent(PauseFinishEvent);
            SetEvent(Event);
        }
    }
}
