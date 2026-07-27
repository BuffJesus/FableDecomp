#include <cstdio>
bool __fastcall CAIStateGroup_Thunder_IsCombatStateGroup()
{
    return false;
}
int main()
{
    if (CAIStateGroup_Thunder_IsCombatStateGroup() == false) { std::printf("AUTO_TINY_009018ec_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009018ec_TEST FAIL\n");
    return 1;
}