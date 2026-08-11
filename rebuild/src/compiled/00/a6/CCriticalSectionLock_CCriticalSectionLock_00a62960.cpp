// CCriticalSectionLock::~CCriticalSectionLock  @ 0x00a62960
// Scoped critical-section guard. If it still holds the lock (flag at +4),
// leave the critical section (pointer at +0) and clear the flag.

struct RTL_CRITICAL_SECTION_stub;

// import thunk: LeaveCriticalSection(LPCRITICAL_SECTION) via IAT (call dword ptr [__imp_...])
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(RTL_CRITICAL_SECTION_stub* cs);

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