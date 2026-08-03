#include <stdio.h>

struct CDisplayEngine;

struct CViewBase {
    char pad[0x24];
    CDisplayEngine* m_pDisplayEngine; // +0x24
};

// Definition of the global the function-under-test reads.
CViewBase* g_pViewBase = 0;

// Function-under-test (same body as source_cpp).
CDisplayEngine* GetDisplayEngine()
{
    return g_pViewBase->m_pDisplayEngine;
}

int main()
{
    // Build a small object; place a sentinel engine pointer at +0x24.
    CViewBase vb;
    CDisplayEngine* sentinel = (CDisplayEngine*)0xDEADBEEF;
    vb.m_pDisplayEngine = sentinel;
    g_pViewBase = &vb;

    CDisplayEngine* got = GetDisplayEngine();
    if (got != sentinel) {
        printf("FAIL got=%p want=%p\n", (void*)got, (void*)sentinel);
        return 1;
    }

    // Also verify it truly reads offset +0x24 (raw byte view).
    unsigned char* raw = (unsigned char*)&vb;
    void* atOff = *(void**)(raw + 0x24);
    if (atOff != (void*)sentinel) {
        printf("FAIL offset atOff=%p\n", atOff);
        return 1;
    }

    printf("OK_0x004c7aa0\n");
    return 0;
}