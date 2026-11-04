#include "fable_critical_section_lock.h"
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) int __stdcall TryEnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
void CCriticalSectionLock::Lock() {
    if (!Locked) { EnterCriticalSection(CriticalSection); Locked = true; }
}
