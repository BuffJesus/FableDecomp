#pragma optimize("s",on)
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