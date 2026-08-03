#include <stdio.h>

struct CNavigatorRadiusHolder
{
    char pad0[0x34];
    float radius;
};

struct CObstacleNavigatorField4
{
    char pad0[0x60];
    CNavigatorRadiusHolder* holder;
};

class CObstacleNavigatorExternalInterface
{
public:
    float __fastcall PeekNavigatorRadius(void* /*unused_edx*/, bool bSomeFlag);

    void* field_0;
    CObstacleNavigatorField4* field_4;
};

float __fastcall CObstacleNavigatorExternalInterface::PeekNavigatorRadius(void* /*unused_edx*/, bool bSomeFlag)
{
    return field_4->holder->radius;
}

int main()
{
    CNavigatorRadiusHolder holder;
    holder.radius = 12.5f;

    CObstacleNavigatorField4 f4;
    f4.holder = &holder;

    CObstacleNavigatorExternalInterface obj;
    obj.field_0 = 0;
    obj.field_4 = &f4;

    float result = obj.PeekNavigatorRadius(0, true);

    if (result == 12.5f)
    {
        printf("PEEK_RADIUS_OK %f\n", result);
    }
    else
    {
        printf("PEEK_RADIUS_FAIL %f\n", result);
    }

    return 0;
}