#include "fable_game.h"
#include <stdio.h>

extern bool g_FableRunUserScript_01375444;
extern bool g_FableFrontendFirstRun_01375448;
extern bool g_FablePlayIntroMovies_0137544A;
extern long g_FableHalfRefreshRate_0137544C;
extern bool g_FableDitheringOn_01375452;
extern bool g_FableUseCompressedTextures_01375453;
extern bool g_FableAllowDataGeneration_01375459;
extern unsigned long g_FableMaximumTextureSize_0137546C;
extern unsigned long g_FableRequestedTextureBitDepth_01375470;
extern long g_FableInstallerBufferSize_01375494;
extern long g_FableNoInstallBuffers_01375498;

int main()
{
    bool* switches[] = {&g_FableRunUserScript_01375444,
                       &g_FableFrontendFirstRun_01375448,
                       &g_FablePlayIntroMovies_0137544A,
                       &g_FableDitheringOn_01375452,
                       &g_FableUseCompressedTextures_01375453,
                       &g_FableAllowDataGeneration_01375459};
    if (g_FableHalfRefreshRate_0137544C != 35 ||
        g_FableMaximumTextureSize_0137546C != 2048 ||
        g_FableRequestedTextureBitDepth_01375470 != 16 ||
        g_FableInstallerBufferSize_01375494 != 0x00180000L ||
        g_FableNoInstallBuffers_01375498 != 12) return 1;
    for (int j = 0; j < 6; ++j) if (!*switches[j]) return 2;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 6; ++j) *switches[j] = ((i + j) & 1) != 0;
        g_FableHalfRefreshRate_0137544C = 35 + i;
        g_FableMaximumTextureSize_0137546C = 2048 + i;
        g_FableRequestedTextureBitDepth_01375470 = 16 + i;
        g_FableInstallerBufferSize_01375494 = 0x00180000L + i;
        g_FableNoInstallBuffers_01375498 = 12 + i;
        for (int j = 0; j < 6; ++j)
            if (*switches[j] != (((i + j) & 1) != 0)) return 3;
        if (g_FableHalfRefreshRate_0137544C != 35 + i ||
            g_FableMaximumTextureSize_0137546C != 2048 + i ||
            g_FableRequestedTextureBitDepth_01375470 != 16 + i ||
            g_FableInstallerBufferSize_01375494 != 0x00180000L + i ||
            g_FableNoInstallBuffers_01375498 != 12 + i) return 4;
    }
    puts("STARTUP_INITIALIZED_SCALARS_PASS cases=16");
    return 0;
}
