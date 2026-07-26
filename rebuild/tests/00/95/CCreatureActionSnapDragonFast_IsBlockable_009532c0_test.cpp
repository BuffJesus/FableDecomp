#include <cstdio>
bool __fastcall CCreatureAction_SnapDragonFast_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_SnapDragonFast_IsBlockable() == false) { std::printf("AUTO_TINY_009532c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009532c0_TEST FAIL\n");
    return 1;
}