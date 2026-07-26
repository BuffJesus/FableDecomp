#include <cstdio>

unsigned long __fastcall CCombatAbilityBlockUnarmedAttackDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x58;
}

int main()
{
    unsigned long r = CCombatAbilityBlockUnarmedAttackDef_GetSizeofClass((void*)0);
    if (r == 0x58) {
        std::printf("CCombatAbilityBlockUnarmedAttackDef_00453e80_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}