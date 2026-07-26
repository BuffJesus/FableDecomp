// Standalone behaviour test for NDisplayView::CViewBase::GetDisplayEngine.
// Compiles with cl (VC7.1) as a normal console app. Provides the extern
// global definition so it links.

#include <stdio.h>

struct CDisplayEngine;

struct CViewGlobal
{
    char _pad0x28[0x28];        // 0x00 .. 0x27
    CDisplayEngine *pEngine;    // 0x28
};

extern CViewGlobal *g_viewBaseGlobal;

CDisplayEngine * __fastcall NDisplayView__CViewBase__GetDisplayEngine(void * /*thisptr*/)
{
    return g_viewBaseGlobal->pEngine;
}

// ---- fixture ----
static CViewGlobal s_global;
CViewGlobal *g_viewBaseGlobal = &s_global;

int main()
{
    // Sentinel engine pointer.
    CDisplayEngine *expected = (CDisplayEngine *)0xDEADBEEF;
    s_global.pEngine = expected;

    // Verify the +0x28 offset is where we wrote it.
    if ((char *)&s_global.pEngine - (char *)&s_global != 0x28)
    {
        printf("offset mismatch\n");
        return 1;
    }

    // Call with an arbitrary (ignored) this pointer.
    char dummyThis[4];
    CDisplayEngine *got = NDisplayView__CViewBase__GetDisplayEngine(dummyThis);

    if (got == expected)
        printf("CVIEWBASE_004c79d0_TEST PASS\n");
    else
        printf("CVIEWBASE_004c79d0_TEST FAIL got=%p\n", (void *)got);

    return 0;
}