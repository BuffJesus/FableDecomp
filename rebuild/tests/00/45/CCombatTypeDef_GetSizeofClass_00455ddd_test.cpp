#include <cstdio>
struct CCombatTypeDef;
unsigned long __fastcall CCombatTypeDef_GetSizeofClass(const CCombatTypeDef* self)
{
    (void)self;
    return 0xacUL;
}
int main()
{
    unsigned long r = CCombatTypeDef_GetSizeofClass((const CCombatTypeDef*)0x12345678);
    if (r == 0xacUL) {
        std::printf("CCombatTypeDef_00455ddd_TEST PASS\n");
        return 0;
    }
    std::printf("CCombatTypeDef_00455ddd_TEST FAIL got %lu\n", r);
    return 1;
}