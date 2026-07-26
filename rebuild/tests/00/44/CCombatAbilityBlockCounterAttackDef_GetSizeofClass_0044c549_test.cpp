#include <cstdio>

struct CCombatAbilityBlockCounterAttackDef { int dummy; };

unsigned long __fastcall CCombatAbilityBlockCounterAttackDef_GetSizeofClass(const CCombatAbilityBlockCounterAttackDef* self)
{
    (void)self;
    return 0x48;
}

int main()
{
    CCombatAbilityBlockCounterAttackDef obj;
    unsigned long r = CCombatAbilityBlockCounterAttackDef_GetSizeofClass(&obj);
    if (r == 0x48) {
        std::printf("CCombatAbilityBlockCounterAttackDef_0044c549_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}