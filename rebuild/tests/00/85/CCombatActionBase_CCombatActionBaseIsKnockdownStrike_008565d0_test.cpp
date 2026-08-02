#include <stdio.h>

class CCombatActionBase
{
public:
    char pad[0xB8];
    unsigned long m_knockdownFlag;

    bool __fastcall IsKnockdownStrike(void* /*edx*/);
};

bool __fastcall CCombatActionBase::IsKnockdownStrike(void* /*edx*/)
{
    if (m_knockdownFlag)
        return true;
    return false;
}

int main()
{
    CCombatActionBase obj;
    obj.m_knockdownFlag = 0;
    bool r1 = obj.IsKnockdownStrike(0);

    obj.m_knockdownFlag = 1;
    bool r2 = obj.IsKnockdownStrike(0);

    obj.m_knockdownFlag = 0x12345678;
    bool r3 = obj.IsKnockdownStrike(0);

    if (r1 == false && r2 == true && r3 == true)
    {
        printf("KNOCKDOWN_STRIKE_OK\n");
    }
    else
    {
        printf("KNOCKDOWN_STRIKE_FAIL %d %d %d\n", r1, r2, r3);
    }
    return 0;
}