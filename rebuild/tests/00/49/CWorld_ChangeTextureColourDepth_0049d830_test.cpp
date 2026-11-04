#include "fable_world.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned char storage[sizeof(CWorld) + 8];
    const unsigned long values[4] = {0, 1, 0x80000000UL, 0xffffffffUL};
    int cases = 0;
    for (int offset = 0; offset != 4; ++offset) {
        for (int n = 0; n != 4; ++n) {
            memset(storage, 0x53 + n, sizeof(storage));
            unsigned char before[sizeof(storage)];
            memcpy(before, storage, sizeof(storage));
            CWorld* world = reinterpret_cast<CWorld*>(storage + offset);
            world->CWorld::ChangeTextureColourDepth(static_cast<long>(values[n]));
            if (memcmp(storage, before, sizeof(storage)) != 0) {
                printf("FAIL offset=%d n=%d\n", offset, n);
                return 1;
            }
            ++cases;
        }
    }
    printf("WORLD_CHANGE_TEXTURE_DEPTH_PASS cases=%d\n", cases);
    return 0;
}
