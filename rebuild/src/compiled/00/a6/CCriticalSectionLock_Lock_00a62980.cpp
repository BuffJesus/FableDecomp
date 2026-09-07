// CCriticalSectionLock_Methods::Lock  @ 00a62980
// Enters the wrapped critical section once (idempotent via a "locked" flag).

#include "engine/CCriticalSectionLock.h"  // retyped onto the PDB layout; byte parity re-verified
typedef void (__stdcall *EnterFn)(void*);

// Import thunk slot at 0x143fe0c (EnterCriticalSection).
extern EnterFn * const g_pfnEnter;
EnterFn * const g_pfnEnter = (EnterFn *)0x0143fe0c;

struct CCriticalSectionLock_Methods : CCriticalSectionLock {
    // +0x04 : already-entered flag

    void Lock();
};

void CCriticalSectionLock_Methods::Lock()
{
    if (!Locked)
    {
        (*g_pfnEnter)(CriticalSection);
        Locked = 1;
    }
}