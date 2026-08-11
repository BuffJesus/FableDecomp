#include <cstdio>

// Redefine the fixed thunk slot at a real, writable location for the test.
typedef void (__stdcall *EnterFn)(void*);

static int g_enterCalls = 0;
static void* g_lastArg = 0;

static void __stdcall FakeEnter(void* p)
{
    g_enterCalls++;
    g_lastArg = p;
}

// A local writable slot standing in for 0x143fe0c.
static EnterFn g_slot = FakeEnter;
EnterFn * const g_pfnEnter = &g_slot;

struct CCriticalSectionLock
{
    void* m_pSection;
    unsigned char m_bLocked;
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

int main()
{
    int csobj = 0;
    void* csptr = &csobj;
    CCriticalSectionLock lk;
    lk.m_pSection = csptr;
    lk.m_bLocked = 0;

    // First lock: should enter once, set flag.
    lk.Lock();
    // Second lock: idempotent, no additional enter.
    lk.Lock();

    bool ok = (g_enterCalls == 1) && (g_lastArg == csptr) && (lk.m_bLocked == 1);

    // Pre-locked object: should not enter.
    CCriticalSectionLock lk2;
    lk2.m_pSection = csptr;
    lk2.m_bLocked = 1;
    lk2.Lock();
    ok = ok && (g_enterCalls == 1);

    printf(ok ? "LOCK_OK\n" : "LOCK_FAIL calls=%d\n", g_enterCalls);
    return ok ? 0 : 1;
}