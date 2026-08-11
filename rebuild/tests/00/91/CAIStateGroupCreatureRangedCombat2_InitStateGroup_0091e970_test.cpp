#include <cstdio>

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

int main()
{
    CAIStateGroup_CreatureRangedCombat2 obj;
    obj.m_flag = 0xAB;
    const CWorld *w = 0;
    CThingAICreature *c = 0;
    obj.InitStateGroup(*w, *c);
    if (obj.m_flag == 0) {
        printf("INITSTATEGROUP_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}