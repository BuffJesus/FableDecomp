#include <cstdio>
bool __fastcall CCreatureAction_BlockBroken_IsActionReadyForInterruption()
{
    return false;
}
int main()
{
    if (CCreatureAction_BlockBroken_IsActionReadyForInterruption() == false) { std::printf("AUTO_TINY_00739a40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00739a40_TEST FAIL\n");
    return 1;
}