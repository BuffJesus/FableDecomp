#include <stdio.h>

struct RTL_CRITICAL_SECTION_stub { int dummy; };

static int g_leaveCount = 0;
static RTL_CRITICAL_SECTION_stub* g_lastCS = 0;

extern "C" void __stdcall LeaveCriticalSection(RTL_CRITICAL_SECTION_stub* cs)
{
    g_leaveCount++;
    g_lastCS = cs;
}

struct CCriticalSectionLock
{
    RTL_CRITICAL_SECTION_stub* m_pCS;  // +0
    unsigned char m_bLocked;           // +4

    ~CCriticalSectionLock();
};

CCriticalSectionLock::~CCriticalSectionLock()
{
    if (m_bLocked)
    {
        LeaveCriticalSection(m_pCS);
        m_bLocked = 0;
    }
}

int main()
{
    RTL_CRITICAL_SECTION_stub cs;

    // Case 1: locked -> should leave and clear flag
    {
        CCriticalSectionLock lk;
        lk.m_pCS = &cs;
        lk.m_bLocked = 1;
    } // dtor here

    // Case 2: not locked -> should NOT leave
    {
        CCriticalSectionLock lk;
        lk.m_pCS = &cs;
        lk.m_bLocked = 0;
    } // dtor here

    if (g_leaveCount == 1 && g_lastCS == &cs)
        printf("DTOR_CS_OK\n");
    else
        printf("DTOR_CS_FAIL count=%d\n", g_leaveCount);

    return (g_leaveCount == 1 && g_lastCS == &cs) ? 0 : 1;
}