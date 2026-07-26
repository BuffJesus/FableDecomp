#include <cstdio>
bool __fastcall CCreatureActionHeroRunningStop_IsActionReadyForInterruption()
{
    return true;
}
int main()
{
    if (CCreatureActionHeroRunningStop_IsActionReadyForInterruption() == true) { std::printf("AUTO_TINY_0085bc10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0085bc10_TEST FAIL\n");
    return 1;
}