#include <cstdio>

static int g_leaveCalls = 0;
static void *g_lastArg = 0;

// Local stub standing in for the imported LeaveCriticalSection.
static void MyLeave(void *pcs)
{
    g_leaveCalls++;
    g_lastArg = pcs;
}

struct CCriticalSectionLock {
    void *pcs;               // +0
    unsigned char locked;    // +4
    ~CCriticalSectionLock();
};

// Same body shape as the authored source, using the local stub.
CCriticalSectionLock::~CCriticalSectionLock()
{
    if (locked) {
        MyLeave(pcs);
        locked = 0;
    }
}

int main()
{
    int fakeCs = 0xABCD;

    // Case 1: locked -> should call leave once with pcs, and clear flag.
    {
        CCriticalSectionLock lock;
        lock.pcs = &fakeCs;
        lock.locked = 1;
        lock.~CCriticalSectionLock();
        if (lock.locked != 0) { printf("FAIL locked-flag\n"); return 1; }
    }
    if (g_leaveCalls != 1) { printf("FAIL leave-count\n"); return 1; }
    if (g_lastArg != &fakeCs) { printf("FAIL leave-arg\n"); return 1; }

    // Case 2: not locked -> no call, flag stays 0.
    {
        CCriticalSectionLock lock;
        lock.pcs = &fakeCs;
        lock.locked = 0;
        lock.~CCriticalSectionLock();
        if (lock.locked != 0) { printf("FAIL unlocked-flag\n"); return 1; }
    }
    if (g_leaveCalls != 1) { printf("FAIL no-extra-call\n"); return 1; }

    printf("DTOR_OK\n");
    return 0;
}