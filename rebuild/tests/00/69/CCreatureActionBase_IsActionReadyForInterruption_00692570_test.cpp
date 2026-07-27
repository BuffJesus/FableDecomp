#include <cstdio>
bool __fastcall CCreatureActionBase_IsActionReadyForInterruption()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsActionReadyForInterruption() == false) { std::printf("AUTO_TINY_00692570_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692570_TEST FAIL\n");
    return 1;
}