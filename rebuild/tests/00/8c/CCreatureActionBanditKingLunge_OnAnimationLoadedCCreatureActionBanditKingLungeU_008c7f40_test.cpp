#include <cstdio>

struct CCreatureAction_Base
{
    char pad[0x10];
    int field10;
    int field14;
    void BaseOnAnimationLoaded();
};

struct CCreatureAction_BanditKingLunge : public CCreatureAction_Base
{
    void OnAnimationLoaded();
};

static int g_baseCalled = 0;
void CCreatureAction_Base::BaseOnAnimationLoaded() { g_baseCalled++; }

void CCreatureAction_BanditKingLunge::OnAnimationLoaded()
{
    BaseOnAnimationLoaded();
    if (field14 < 0)
        field14 = field10 - 5;
}

int main()
{
    CCreatureAction_BanditKingLunge a;
    a.field10 = 100;

    // case 1: field14 negative -> gets field10 - 5
    a.field14 = -1;
    a.OnAnimationLoaded();
    bool ok1 = (a.field14 == 95) && (g_baseCalled == 1);

    // case 2: field14 non-negative -> unchanged
    a.field14 = 7;
    a.OnAnimationLoaded();
    bool ok2 = (a.field14 == 7) && (g_baseCalled == 2);

    if (ok1 && ok2)
        printf("BANDITKINGLUNGE_ANIM_OK\n");
    else
        printf("FAIL %d %d\n", a.field14, g_baseCalled);
    return (ok1 && ok2) ? 0 : 1;
}