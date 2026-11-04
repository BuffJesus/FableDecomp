#include "fable_world.h"
#include <stdio.h>
#include <string.h>

struct TestWorld : CWorld
{
    static void call(CWorld* world)
    {
        reinterpret_cast<TestWorld*>(world)->CWorld::PostLoadInit();
    }
};

int main()
{
    unsigned char storage[sizeof(CWorld) + 8];
    int cases = 0;
    for (int offset = 0; offset != 4; ++offset) {
        for (int initial = 0; initial != 4; ++initial) {
            memset(storage, 0xa5, sizeof(storage));
            CWorld* world = reinterpret_cast<CWorld*>(storage + offset);
            reinterpret_cast<unsigned char*>(world)[0x80] = static_cast<unsigned char>(initial * 83);
            TestWorld::call(world);
            if (reinterpret_cast<unsigned char*>(world)[0x80] != 1) {
                printf("FAIL offset=%d initial=%d\n", offset, initial);
                return 1;
            }
            ++cases;
        }
    }
    printf("WORLD_POST_LOAD_INIT_PASS cases=%d\n", cases);
    return 0;
}
