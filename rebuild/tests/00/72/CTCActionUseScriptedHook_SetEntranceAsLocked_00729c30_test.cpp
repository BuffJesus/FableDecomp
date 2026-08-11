#include <cstdio>

struct Entrance
{
    void* Resolve();
    char data[8];
};

struct CTCActionUseScriptedHook
{
    char pad0[0x20];
    Entrance m_entrance;   // +0x20
    char m_locked;         // +0x28

    void SetEntranceAsLocked(bool locked);
};

static void* g_resolveResult = 0;

void* Entrance::Resolve()
{
    return g_resolveResult;
}

int main()
{
    CTCActionUseScriptedHook obj;

    // case 1: Resolve returns null -> locked NOT written
    obj.m_locked = 7;
    g_resolveResult = 0;
    obj.SetEntranceAsLocked(true);
    bool ok1 = (obj.m_locked == 7);

    // case 2: Resolve non-null -> locked written = true(1)
    obj.m_locked = 7;
    g_resolveResult = (void*)0x1234;
    obj.SetEntranceAsLocked(true);
    bool ok2 = (obj.m_locked == 1);

    // case 3: Resolve non-null, locked=false -> 0
    obj.m_locked = 7;
    obj.SetEntranceAsLocked(false);
    bool ok3 = (obj.m_locked == 0);

    // offset sanity
    bool ok4 = ((char*)&obj.m_locked - (char*)&obj) == 0x28;

    if (ok1 && ok2 && ok3 && ok4)
        printf("LOCK_PARITY_OK\n");
    else
        printf("FAIL %d %d %d %d\n", ok1, ok2, ok3, ok4);
    return (ok1 && ok2 && ok3 && ok4) ? 0 : 1;
}