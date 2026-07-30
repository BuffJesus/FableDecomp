#include <cstdio>

// definition of the global the function-under-test writes
void *g_CurrentThingManager = 0;

void __fastcall SetCurrentThingManager(void *pManager)
{
    g_CurrentThingManager = pManager;
}

int main()
{
    void *marker = (void *)0xDEADBEEF;

    // invoke with this/arg in ecx via a small __asm shim
    __asm
    {
        mov ecx, marker
        call SetCurrentThingManager
    }

    if (g_CurrentThingManager != marker)
    {
        printf("FAIL got=%p want=%p\n", g_CurrentThingManager, marker);
        return 1;
    }

    // second value to prove it truly writes ecx each time
    marker = (void *)0x12345678;
    __asm
    {
        mov ecx, marker
        call SetCurrentThingManager
    }
    if (g_CurrentThingManager != marker)
    {
        printf("FAIL2 got=%p\n", g_CurrentThingManager);
        return 1;
    }

    printf("OK_0x004c74f0\n");
    return 0;
}