#include "fable_config_detection.h"

bool FABLE_FASTCALL GFFreeConfigDetection()
{
    FableConfigReleaseFunction release =
        reinterpret_cast<FableConfigReleaseFunction>(
            g_FableGetProcAddress_0143FE44(
                g_FableConfigDetectionModule_013B7C8C,
                "Config_Release"));
    if (release != 0)
    {
        release();
    }

    g_FableFreeLibrary_0143FE3C(
        g_FableConfigDetectionModule_013B7C8C);
    g_FableConfigDetectionModule_013B7C8C = 0;
    return false;
}
