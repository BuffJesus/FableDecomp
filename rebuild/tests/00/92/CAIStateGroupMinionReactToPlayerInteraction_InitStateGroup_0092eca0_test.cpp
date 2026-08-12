#include <cstdio>

class CWorld;
class CThingAICreature;

struct CAIStateGroup_MinionReactToPlayerInteraction
{
    char pad[0x14];
    unsigned int m_14;
    unsigned int m_18;
    unsigned int m_1c;
    unsigned int m_20;
    unsigned int m_24;
    unsigned int m_28;
    unsigned char m_2c;
    unsigned char m_2d;

    void InitStateGroup(CWorld const& world, CThingAICreature& creature);
};

void CAIStateGroup_MinionReactToPlayerInteraction::InitStateGroup(
    CWorld const&, CThingAICreature&)
{
    m_14 = 0;
    m_18 = 0;
    m_1c = 0;
    m_20 = 0;
    m_24 = 0;
    m_28 = 0;
    m_2c = 0;
    m_2d = 0;
}

int main()
{
    CAIStateGroup_MinionReactToPlayerInteraction g;
    // preload with junk
    g.m_14 = 0x11111111; g.m_18 = 0x22222222; g.m_1c = 0x33333333;
    g.m_20 = 0x44444444; g.m_24 = 0x55555555; g.m_28 = 0x66666666;
    g.m_2c = 0x77; g.m_2d = 0x88;

    CWorld* w = 0;
    CThingAICreature* c = 0;
    g.InitStateGroup(*w, *c);

    bool ok = g.m_14==0 && g.m_18==0 && g.m_1c==0 && g.m_20==0 &&
              g.m_24==0 && g.m_28==0 && g.m_2c==0 && g.m_2d==0;
    if (ok) { printf("INITSG_OK\n"); return 0; }
    printf("INITSG_FAIL\n");
    return 1;
}