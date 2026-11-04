#include "fable_world.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned char worldStorage[sizeof(CWorld) + 8];
    unsigned char environmentStorage[64];
    int cases = 0;
    for (int offset = 0; offset != 4; ++offset) {
        for (int state = 0; state != 3; ++state) {
            memset(worldStorage, 0xa5, sizeof(worldStorage));
            CWorld* world = reinterpret_cast<CWorld*>(worldStorage + offset);
            CEnvironment* environment = state ?
                reinterpret_cast<CEnvironment*>(environmentStorage + state * 4) : 0;
            *reinterpret_cast<CEnvironment**>(reinterpret_cast<char*>(world) + 0x18) = environment;
            const CIDrawEnvironment* got = world->CWorld::DrawGetEnvironment();
            const CIDrawEnvironment* expected =
                reinterpret_cast<const CIDrawEnvironment*>(environment);
            if (got != expected) {
                printf("FAIL offset=%d state=%d got=%p expected=%p\n", offset, state, got, expected);
                return 1;
            }
            ++cases;
        }
    }
    printf("WORLD_DRAW_ENVIRONMENT_PASS cases=%d\n", cases);
    return 0;
}
