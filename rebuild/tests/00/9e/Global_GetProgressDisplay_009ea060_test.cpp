#include <stdio.h>

#include "fable_boot.h"

CCountedProgressDisplay g_FableProgressDisplay_013CAA38;

namespace
{
    int g_destroyCalls = 0;
    int g_deleteCalls = 0;

    void FABLE_FASTCALL DestroyDisplay(void* /* object */)
    {
        ++g_destroyCalls;
    }
}

void FABLE_CDECL FableOperatorDelete(void* /* allocation */)
{
    ++g_deleteCalls;
}

int main()
{
    fable_u8 displayStorage[sizeof(CProgressDisplay)] = {};
    CProgressDisplay* display =
        reinterpret_cast<CProgressDisplay*>(displayStorage);
    FableReferenceCount reference = {2, DestroyDisplay, display};
    g_FableProgressDisplay_013CAA38.object = display;
    g_FableProgressDisplay_013CAA38.reference = &reference;

    {
        CCountedProgressDisplay result = GetProgressDisplay();
        if (
            result.object != display ||
            result.reference != &reference ||
            reference.owners != 3 ||
            g_FableProgressDisplay_013CAA38.object != display ||
            g_FableProgressDisplay_013CAA38.reference != &reference)
        {
            return 1;
        }
    }
    if (reference.owners != 2 || g_destroyCalls != 0 || g_deleteCalls != 0)
        return 2;

    g_FableProgressDisplay_013CAA38.object = 0;
    g_FableProgressDisplay_013CAA38.reference = 0;
    {
        CCountedProgressDisplay empty = GetProgressDisplay();
        if (empty.object != 0 || empty.reference != 0)
            return 3;
    }
    if (reference.owners != 2 || g_destroyCalls != 0 || g_deleteCalls != 0)
        return 4;

    printf("FABLETLC_GET_PROGRESS_DISPLAY_BEHAVIOR PASS\n");
    return 0;
}
