#include <cstdio>
bool __fastcall CCombatActionBase_IsActionReadyForInterruptionByMovement(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CCombatActionBase_IsActionReadyForInterruptionByMovement(&object, 1, 2)) { std::printf("AUTO_TINY_00856190_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00856190_TEST FAIL\n");
    return 1;
}