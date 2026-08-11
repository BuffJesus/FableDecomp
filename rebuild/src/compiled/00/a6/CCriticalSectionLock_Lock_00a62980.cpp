// CCriticalSectionLock::Lock  @ 00a62980
// Enters the wrapped critical section once (idempotent via a "locked" flag).

typedef void (__stdcall *EnterFn)(void*);

// Import thunk slot at 0x143fe0c (EnterCriticalSection).
extern EnterFn * const g_pfnEnter;
EnterFn * const g_pfnEnter = (EnterFn *)0x0143fe0c;

struct CCriticalSectionLock
{
    void* m_pSection;   // +0x00 : CRITICAL_SECTION*
    unsigned char m_bLocked; // +0x04 : already-entered flag

    void Lock();
};

void CCriticalSectionLock::Lock()
{
    if (!m_bLocked)
    {
        (*g_pfnEnter)(m_pSection);
        m_bLocked = 1;
    }
}