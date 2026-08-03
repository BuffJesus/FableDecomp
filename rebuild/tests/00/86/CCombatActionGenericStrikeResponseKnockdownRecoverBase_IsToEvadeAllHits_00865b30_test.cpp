#include <cstdio>

class CCombatAction_GenericStrikeResponseKnockdownRecoverBase
{
public:
    char pad_0000[0x114];
    unsigned char m_ToEvadeAllHits;

    unsigned char __fastcall IsToEvadeAllHits(void*);
};

unsigned char __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverBase::IsToEvadeAllHits(void*)
{
    return m_ToEvadeAllHits == 0;
}

int main()
{
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase obj;

    obj.m_ToEvadeAllHits = 0;
    unsigned char r1 = obj.IsToEvadeAllHits(0);

    obj.m_ToEvadeAllHits = 1;
    unsigned char r2 = obj.IsToEvadeAllHits(0);

    obj.m_ToEvadeAllHits = 0xFF;
    unsigned char r3 = obj.IsToEvadeAllHits(0);

    if (r1 == 1 && r2 == 0 && r3 == 0)
    {
        printf("IsToEvadeAllHits_TEST_OK\n");
    }
    else
    {
        printf("IsToEvadeAllHits_TEST_FAIL %d %d %d\n", (int)r1, (int)r2, (int)r3);
    }

    return 0;
}