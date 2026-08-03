#include <cstdio>

class CCombatActionBase
{
public:
    char pad[0xB8];
    unsigned long m_meleeFlourish;

    bool IsMeleeFlourish();
};

bool CCombatActionBase::IsMeleeFlourish()
{
    if (m_meleeFlourish)
        return true;
    return false;
}

int main()
{
    CCombatActionBase obj;
    for (unsigned int i = 0; i < sizeof(obj); ++i)
        ((char*)&obj)[i] = 0;

    obj.m_meleeFlourish = 0;
    bool r1 = obj.IsMeleeFlourish();

    obj.m_meleeFlourish = 1;
    bool r2 = obj.IsMeleeFlourish();

    obj.m_meleeFlourish = 0xFFFFFFFF;
    bool r3 = obj.IsMeleeFlourish();

    obj.m_meleeFlourish = 0x100;
    bool r4 = obj.IsMeleeFlourish();

    if (r1 == false && r2 == true && r3 == true && r4 == true)
        printf("MELEE_FLOURISH_TEST_OK\n");
    else
        printf("MELEE_FLOURISH_TEST_FAIL\n");

    return 0;
}