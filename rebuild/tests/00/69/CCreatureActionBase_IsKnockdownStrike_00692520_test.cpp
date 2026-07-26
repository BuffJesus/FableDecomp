#include <cstdio>
bool __fastcall CCreatureActionBase_IsKnockdownStrike()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsKnockdownStrike() == false) { std::printf("AUTO_TINY_00692520_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692520_TEST FAIL\n");
    return 1;
}