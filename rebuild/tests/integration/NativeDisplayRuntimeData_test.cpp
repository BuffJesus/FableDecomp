#include "fable_system_activation.h"
#include "fable_virtual_pixels.h"
#include "fable_window_procedure.h"
#include <stdio.h>

int main()
{
    if (FableWindowGamma_0138e0b8 != 1.0f || FableSavedSoundVolume_013ca60c != 0.0f ||
        FableSoundDeactivated_013ca610 || FableWindowSystemManager_013ca798 ||
        FableVirtualPixelScaleX_013ca800 != 0.0 || FableVirtualPixelScaleY_013ca808 != 0.0) return 1;
    for (int i = 0; i != 16; ++i)
    {
        FableWindowGamma_0138e0b8 = 1.0f + i;
        FableSavedSoundVolume_013ca60c = 0.5f + i;
        FableSoundDeactivated_013ca610 = (i & 1) != 0;
        FableWindowSystemManager_013ca798 = reinterpret_cast<CSystemManager*>(0x1000 + i * 4);
        FableVirtualPixelScaleX_013ca800 = 128.0 + i;
        FableVirtualPixelScaleY_013ca808 = 256.0 + i;
        if (FableWindowGamma_0138e0b8 != 1.0f + i ||
            FableSavedSoundVolume_013ca60c != 0.5f + i ||
            FableSoundDeactivated_013ca610 != ((i & 1) != 0) ||
            FableWindowSystemManager_013ca798 != reinterpret_cast<CSystemManager*>(0x1000 + i * 4) ||
            FableVirtualPixelScaleX_013ca800 != 128.0 + i ||
            FableVirtualPixelScaleY_013ca808 != 256.0 + i) return 2;
    }
    puts("NATIVE_DISPLAY_RUNTIME_DATA_PASS cases=16");
    return 0;
}
