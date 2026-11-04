#include "fable_world.h"
#include <stdio.h>

int main()
{
    int cases = 0;
    for (unsigned long p = 0; p != 32; ++p) {
        CWorld* world = reinterpret_cast<CWorld*>(0x10000 + p * 4);
        if (world->CWorld::DrawGetProceduralTexture() != 0) {
            printf("FAIL case=%lu\n", p);
            return 1;
        }
        ++cases;
    }
    printf("WORLD_DRAW_PROCEDURAL_PASS cases=%d\n", cases);
    return 0;
}
