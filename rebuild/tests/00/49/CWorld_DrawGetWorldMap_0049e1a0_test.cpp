#include "fable_world.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned char worldStorage[sizeof(CWorld) + 8];
    unsigned char mapStorage[64];
    int cases = 0;
    for (int offset = 0; offset != 4; ++offset) {
        for (int state = 0; state != 3; ++state) {
            memset(worldStorage, 0xa5, sizeof(worldStorage));
            CWorld* world = reinterpret_cast<CWorld*>(worldStorage + offset);
            CWorldMap* map = state ? reinterpret_cast<CWorldMap*>(mapStorage + state * 4) : 0;
            *reinterpret_cast<CWorldMap**>(reinterpret_cast<char*>(world) + 0x14) = map;
            const CIDrawWorldMap* got = world->CWorld::DrawGetWorldMap();
            const CIDrawWorldMap* expected = map ?
                reinterpret_cast<const CIDrawWorldMap*>(reinterpret_cast<const char*>(map) + 4) : 0;
            if (got != expected) {
                printf("FAIL offset=%d state=%d got=%p expected=%p\n", offset, state, got, expected);
                return 1;
            }
            ++cases;
        }
    }
    printf("WORLD_DRAW_MAP_PASS cases=%d\n", cases);
    return 0;
}
