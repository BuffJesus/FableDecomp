#include <stdio.h>
#include <string.h>
#include <float.h>
#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"

double g_FableVirtualWidth_013961E8, g_FableVirtualHeight_013961F0;

int main(int argc, char** argv)
{
    if (argc != 3) return 2;
    FILE* input = fopen(argv[1], "rb");
    FILE* output = fopen(argv[2], "wb");
    if (!input || !output) return 3;
    unsigned char storage[sizeof(CDisplayManager)], before[sizeof(storage)];
    const unsigned modes[] = {_RC_NEAR, _RC_DOWN, _RC_UP, _RC_CHOP};
    unsigned mode, cases = 0;
    while (fread(&mode, 4, 1, input) == 1) {
        if (mode >= 4 || fread(&g_FableVirtualWidth_013961E8, 8, 1, input) != 1 ||
            fread(&g_FableVirtualHeight_013961F0, 8, 1, input) != 1) return 4;
        memset(storage, (cases * 37) & 255, sizeof(storage));
        memcpy(before, storage, sizeof(storage));
        _controlfp(modes[mode], _MCW_RC);
        const C2DBoxF result = reinterpret_cast<CDisplayManager*>(storage)->GetVVisibleTelevisionScreenArea();
        if (memcmp(storage, before, sizeof(storage))) return 5;
        if (fwrite(&result, sizeof(result), 1, output) != 1) return 6;
        ++cases;
    }
    fclose(input);
    if (fclose(output)) return 7;
    printf("NATIVE_VISIBLE_AREA_PASS cases=%u\n", cases);
    return 0;
}
