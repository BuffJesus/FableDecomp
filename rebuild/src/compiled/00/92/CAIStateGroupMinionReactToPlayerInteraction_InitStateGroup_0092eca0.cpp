// CAIStateGroup_MinionReactToPlayerInteraction::InitStateGroup  @ 0x0092eca0
// __fastcall(void) with (CWorld const&, CThingAICreature&) -> ret 8

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