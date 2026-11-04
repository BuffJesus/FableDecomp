#include "fable_game.h"
#include <stdio.h>

extern unsigned long g_FableMaxTextureHeight_013B7D60;
extern unsigned long g_FableMaxTextureWidth_013B7D64;
extern unsigned long g_FableTextureBitDepth_013B7D68;

int main()
{
    if (g_DeleteCheckPointSaveOnInitWorld_013B7C90 ||
        g_FableMaxTextureHeight_013B7D60 ||
        g_FableMaxTextureWidth_013B7D64 ||
        g_FableTextureBitDepth_013B7D68 ||
        g_RenderOnlyWhenRecordingMovie_013B860C || g_RecordMovie_013B8629)
        return 1;
    g_DeleteCheckPointSaveOnInitWorld_013B7C90 = true;
    g_FableMaxTextureHeight_013B7D60 = 1024;
    g_FableMaxTextureWidth_013B7D64 = 2048;
    g_FableTextureBitDepth_013B7D68 = 32;
    g_RenderOnlyWhenRecordingMovie_013B860C = true;
    g_RecordMovie_013B8629 = true;
    if (!g_DeleteCheckPointSaveOnInitWorld_013B7C90 ||
        g_FableMaxTextureHeight_013B7D60 != 1024 ||
        g_FableMaxTextureWidth_013B7D64 != 2048 ||
        g_FableTextureBitDepth_013B7D68 != 32 ||
        !g_RenderOnlyWhenRecordingMovie_013B860C || !g_RecordMovie_013B8629)
        return 2;
    g_DeleteCheckPointSaveOnInitWorld_013B7C90 = false;
    g_FableMaxTextureHeight_013B7D60 = 0;
    g_FableMaxTextureWidth_013B7D64 = 0;
    g_FableTextureBitDepth_013B7D68 = 0;
    g_RenderOnlyWhenRecordingMovie_013B860C = false;
    g_RecordMovie_013B8629 = false;
    puts("STARTUP_GRAPHICS_RECORDING_DATA_PASS cases=18");
    return 0;
}
