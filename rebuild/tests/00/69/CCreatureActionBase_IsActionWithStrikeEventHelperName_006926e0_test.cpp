#include <cstdio>
bool __fastcall CCreatureActionBase_IsActionWithStrikeEventHelperName()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsActionWithStrikeEventHelperName() == false) { std::printf("AUTO_TINY_006926e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006926e0_TEST FAIL\n");
    return 1;
}