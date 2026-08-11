class CWorld;
class CThingAICreature;

struct CAIStateGroup_CreatureRangedCombat2 {
    char pad[0x1c];
    unsigned char m_flag;
    void InitStateGroup(const CWorld &world, CThingAICreature &creature);
};

void CAIStateGroup_CreatureRangedCombat2::InitStateGroup(const CWorld &world, CThingAICreature &creature)
{
    this->m_flag = 0;
}