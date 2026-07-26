#include "fable_startup.h"

void FABLE_FASTCALL FableClearStartupLatch_013964A8()
{
    if (g_FableStartupLatch_013964A8)
        g_FableStartupLatch_013964A8 = false;
}
