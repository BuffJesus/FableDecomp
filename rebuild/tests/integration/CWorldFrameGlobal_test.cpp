#include "fable_world.h"
#include <stdio.h>

struct CWorldFrameAccess : public CWorld
{
    static void Set(long value) { Frame = value; }
    static long Get() { return Frame; }
};

int main()
{
    if (CWorldFrameAccess::Get() != 0 || CWorld::GetFrame() != 0) return 1;
    const long values[] = {0, 1, -1, 7, -7, 0x12345678L,
                           0x7fffffffL, static_cast<long>(0x80000000UL)};
    int cases = 0;
    for (unsigned int i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
    {
        CWorldFrameAccess::Set(values[i]);
        if (CWorldFrameAccess::Get() != values[i] || CWorld::GetFrame() != values[i]) return 2;
        ++cases;
    }
    CWorldFrameAccess::Set(0);
    printf("WORLD_FRAME_GLOBAL_PASS cases=%d\n", cases);
    return 0;
}
