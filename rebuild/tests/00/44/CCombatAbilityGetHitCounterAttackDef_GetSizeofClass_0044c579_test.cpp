#include <cstdio>

struct CCombatAbilityGetHitCounterAttackDef { int dummy; };

unsigned long __fastcall CCombatAbilityGetHitCounterAttackDef_GetSizeofClass(const CCombatAbilityGetHitCounterAttackDef* self)
{
    return 0x48;
}

int main()
{
    CCombatAbilityGetHitCounterAttackDef obj;
    obj.dummy = 0;
    unsigned long r = CCombatAbilityGetHitCounterAttackDef_GetSizeofClass(&obj);
    if (r == 0x48) {
        std::printf("CCombatAbilityGetHitCounterAttackDef_0044c579_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}