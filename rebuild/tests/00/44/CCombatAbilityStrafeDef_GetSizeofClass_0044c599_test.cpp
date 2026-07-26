#include <cstdio>

struct CCombatAbilityStrafeDef { int dummy; };

unsigned long __fastcall CCombatAbilityStrafeDef_GetSizeofClass(const CCombatAbilityStrafeDef* self)
{
    (void)self;
    return 0x44;
}

int main()
{
    CCombatAbilityStrafeDef obj;
    unsigned long r = CCombatAbilityStrafeDef_GetSizeofClass(&obj);
    if (r == 0x44) {
        std::printf("CCombatAbilityStrafeDef_0044c599_TEST PASS\n");
        return 0;
    }
    std::printf("CCombatAbilityStrafeDef_0044c599_TEST FAIL got=%lu\n", r);
    return 1;
}