#include "fable_gfuninitialise.h"
#include <stdio.h>

extern void* g_FableRenderState_013B8380;

int main()
{
    if (g_FableGraphicBankManager_013B837C || g_FableRenderState_013B8380 ||
        g_FableRenderManager2D_013B8384 || g_FableInputManager_013B8388 ||
        g_FableFontManager_013B838C || g_FableDisplayManager_013B8390 ||
        g_FableSoundManager_013B8394) return 1;
    unsigned char storage[7][16];
    for (int i = 0; i < 16; ++i) {
        g_FableGraphicBankManager_013B837C = reinterpret_cast<CGraphicBankManager*>(storage[0] + i);
        g_FableRenderState_013B8380 = storage[1] + i;
        g_FableRenderManager2D_013B8384 = reinterpret_cast<CRenderManager2D*>(storage[2] + i);
        g_FableInputManager_013B8388 = reinterpret_cast<CInputManager*>(storage[3] + i);
        g_FableFontManager_013B838C = reinterpret_cast<CFontManager*>(storage[4] + i);
        g_FableDisplayManager_013B8390 = reinterpret_cast<CDisplayManager*>(storage[5] + i);
        g_FableSoundManager_013B8394 = reinterpret_cast<CASoundManager*>(storage[6] + i);
        if (reinterpret_cast<void*>(g_FableGraphicBankManager_013B837C) != storage[0] + i ||
            g_FableRenderState_013B8380 != storage[1] + i ||
            reinterpret_cast<void*>(g_FableRenderManager2D_013B8384) != storage[2] + i ||
            reinterpret_cast<void*>(g_FableInputManager_013B8388) != storage[3] + i ||
            reinterpret_cast<void*>(g_FableFontManager_013B838C) != storage[4] + i ||
            reinterpret_cast<void*>(g_FableDisplayManager_013B8390) != storage[5] + i ||
            reinterpret_cast<void*>(g_FableSoundManager_013B8394) != storage[6] + i) return 2;
    }
    puts("STARTUP_CACHE_DATA_PASS cases=16");
    return 0;
}
