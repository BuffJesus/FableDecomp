#include "fable_critical_section_lock.h"
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) int __stdcall TryEnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryOnly) : CriticalSection(&section) {
    if (tryOnly) Locked = TryEnterCriticalSection(CriticalSection) != 0;
    else { EnterCriticalSection(CriticalSection); Locked = true; }
}
