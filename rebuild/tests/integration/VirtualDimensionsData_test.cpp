#include "fable_virtual_pixels.h"
#include <stdio.h>

int main()
{
    if (g_FableVirtualWidth_013961E8 != 65536.0 ||
        g_FableVirtualHeight_013961F0 != 65536.0) return 1;
    for (int i = 0; i < 16; ++i) {
        g_FableVirtualWidth_013961E8 = 640.0 + i;
        g_FableVirtualHeight_013961F0 = 480.0 + 2 * i;
        if (g_FableVirtualWidth_013961E8 != 640.0 + i ||
            g_FableVirtualHeight_013961F0 != 480.0 + 2 * i) return 2;
    }
    puts("VIRTUAL_DIMENSIONS_DATA_PASS cases=16");
    return 0;
}
