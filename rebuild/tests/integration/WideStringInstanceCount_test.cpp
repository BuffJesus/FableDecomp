#include <stdio.h>
#include "fable_string.h"

extern "C" long g_charStringRefTrace;

int main()
{
    if (sizeof(g_CWideStringInstanceCount_013BCA20) != 4) return 1;
    if (static_cast<void*>(&g_CWideStringInstanceCount_013BCA20) !=
        static_cast<void*>(&g_charStringRefTrace)) return 2;
    volatile long* legacy = &g_charStringRefTrace;
    volatile fable_i32* typed = &g_CWideStringInstanceCount_013BCA20;
    if (*legacy != 0) return 3;
    *legacy = 7;
    if (*typed != 7) return 4;
    *typed = 0;
    puts("WIDESTRING_INSTANCE_COUNT_PASS");
    return 0;
}
