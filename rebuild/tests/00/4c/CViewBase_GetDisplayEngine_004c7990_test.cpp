// Standalone behaviour test for NDisplayView::CViewBase::GetDisplayEngine @ 0x004c7990
// Build: cl /O2 /Oy /W3 test.cpp
#include <cstdio>

struct CDisplayEngine;

struct DisplaySingleton {
    char _pad[0x20];
    CDisplayEngine *displayEngine;
};

DisplaySingleton *g_displaySingleton = 0;

struct CViewBase {
    CDisplayEngine *GetDisplayEngine();
};

CDisplayEngine *CViewBase::GetDisplayEngine()
{
    return g_displaySingleton->displayEngine;
}

int main()
{
    // Fixture: the global singleton points at a controlled object whose
    // +0x20 field holds a known sentinel display-engine pointer.
    DisplaySingleton s;
    CDisplayEngine *sentinel = (CDisplayEngine *)0xDEADBEEF;
    s.displayEngine = sentinel;
    g_displaySingleton = &s;

    // 'this' is irrelevant to the accessor; use any object.
    CViewBase view;
    CDisplayEngine *got = view.GetDisplayEngine();

    // Offset check: field must sit exactly at +0x20.
    if ((char *)&s.displayEngine - (char *)&s != 0x20) {
        printf("FAIL: bad offset %d\n",
               (int)((char *)&s.displayEngine - (char *)&s));
        return 1;
    }

    if (got == sentinel) {
        printf("CVIEWBASE_004c7990_TEST PASS\n");
        return 0;
    }
    printf("FAIL: got %p want %p\n", (void *)got, (void *)sentinel);
    return 1;
}