// CCreatureAction_BanditKingLunge::OnAnimationLoaded  @ 0x008c7f40

struct CCreatureAction_Base
{
    char pad[0x10];   // 0x00 .. 0x0f
    int field10;      // 0x10
    int field14;      // 0x14
    void BaseOnAnimationLoaded();  // -> 0x859ef0
};

struct CCreatureAction_BanditKingLunge : public CCreatureAction_Base
{
    void OnAnimationLoaded();
};

void CCreatureAction_BanditKingLunge::OnAnimationLoaded()
{
    BaseOnAnimationLoaded();
    if (field14 < 0)
        field14 = field10 - 5;
}