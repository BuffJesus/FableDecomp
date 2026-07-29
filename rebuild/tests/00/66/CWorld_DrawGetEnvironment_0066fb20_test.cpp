#include <stdio.h>

struct CIDrawEnvironment
{
    unsigned long marker;
};

struct CWorld_0066fb20
{
    unsigned char prefix[0x10];
    CIDrawEnvironment* draw_environment;
};

extern "C" CIDrawEnvironment* __fastcall
CWorld_DrawGetEnvironment_0066fb20(CWorld_0066fb20* self);

int main()
{
    CWorld_0066fb20 world;
    CIDrawEnvironment environment;

    environment.marker = 0x12345678;
    world.draw_environment = &environment;

    if (CWorld_DrawGetEnvironment_0066fb20(&world) != &environment)
    {
        puts("FAIL: getter must return the exact environment pointer");
        return 1;
    }

    world.draw_environment = 0;
    if (CWorld_DrawGetEnvironment_0066fb20(&world) != 0)
    {
        puts("FAIL: getter must preserve a null environment pointer");
        return 1;
    }

    puts("PASS CWorld::DrawGetEnvironment 0x0066FB20");
    return 0;
}
