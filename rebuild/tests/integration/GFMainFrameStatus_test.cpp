#include "fable_game.h"
#include <stdio.h>

extern unsigned long g_FableLanguage_013B8684;
extern bool g_FableSaveGameLaunchFailed_013B89C0;

int main()
{
    if (g_RenderFrame_013B7D6C || g_WorldFrame_013B7D70 ||
        g_ForceRenderFramesPerSec_013B8630 || g_FableLanguage_013B8684 ||
        g_FableSaveGameLaunchFailed_013B89C0) return 1;
    for (int i = 0; i < 16; ++i) {
        g_RenderFrame_013B7D6C = 100 + i;
        g_WorldFrame_013B7D70 = 200 + i;
        g_ForceRenderFramesPerSec_013B8630 = i - 8;
        g_FableLanguage_013B8684 = i;
        g_FableSaveGameLaunchFailed_013B89C0 = (i & 1) != 0;
        if (g_RenderFrame_013B7D6C != 100 + i ||
            g_WorldFrame_013B7D70 != 200 + i ||
            g_ForceRenderFramesPerSec_013B8630 != i - 8 ||
            g_FableLanguage_013B8684 != static_cast<unsigned long>(i) ||
            g_FableSaveGameLaunchFailed_013B89C0 != ((i & 1) != 0)) return 2;
    }
    puts("GFMAIN_FRAME_STATUS_PASS cases=16");
    return 0;
}
