#include <stdio.h>

// Global the function-under-test writes to.
void *g_CurrentThingManager = 0;

// Function-under-test: __fastcall stores ecx (single ptr arg) into global.
void __fastcall SetCurrentThingManager(void *mgr)
{
    g_CurrentThingManager = mgr;
}

int main()
{
    void *probe = (void *)0xDEADBEEF;

    // __fastcall takes first arg in ecx; call normally (compiler passes in ecx),
    // but also exercise via an explicit __asm shim to prove the ecx contract.
    g_CurrentThingManager = 0;
    __asm {
        mov ecx, probe
        call SetCurrentThingManager
    }

    if (g_CurrentThingManager != probe) {
        printf("MISMATCH got=%p want=%p\n", g_CurrentThingManager, probe);
        return 1;
    }

    // Second call with a different value, via normal fastcall call convention.
    void *probe2 = (void *)0x12345678;
    SetCurrentThingManager(probe2);
    if (g_CurrentThingManager != probe2) {
        printf("MISMATCH2 got=%p want=%p\n", g_CurrentThingManager, probe2);
        return 1;
    }

    printf("OK_0x004bbfc0 stored=%p\n", g_CurrentThingManager);
    return 0;
}