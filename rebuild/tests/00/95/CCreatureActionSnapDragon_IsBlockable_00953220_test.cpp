#include <cstdio>
bool __fastcall CCreatureAction_SnapDragon_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_SnapDragon_IsBlockable() == false) { std::printf("AUTO_TINY_00953220_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00953220_TEST FAIL\n");
    return 1;
}