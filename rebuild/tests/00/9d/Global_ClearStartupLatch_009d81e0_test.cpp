#include <stdio.h>

#include "fable_startup.h"

bool g_FableStartupLatch_013964A8 = false;

int main()
{
    FableClearStartupLatch_013964A8();
    if (g_FableStartupLatch_013964A8)
    {
        printf("FABLETLC_STARTUP_LATCH_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    g_FableStartupLatch_013964A8 = true;
    FableClearStartupLatch_013964A8();
    if (g_FableStartupLatch_013964A8)
    {
        printf("FABLETLC_STARTUP_LATCH_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_STARTUP_LATCH_BEHAVIOR PASS\n");
    return 0;
}
