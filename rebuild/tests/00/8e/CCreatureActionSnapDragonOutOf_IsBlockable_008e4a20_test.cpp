#include <cstdio>
bool __fastcall CCreatureAction_SnapDragonOutOf_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_SnapDragonOutOf_IsBlockable() == false) { std::printf("AUTO_TINY_008e4a20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008e4a20_TEST FAIL\n");
    return 1;
}