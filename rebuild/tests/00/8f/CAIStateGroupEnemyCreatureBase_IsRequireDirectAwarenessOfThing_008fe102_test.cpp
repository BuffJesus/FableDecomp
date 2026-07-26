#include <cstdio>
bool __fastcall CAIStateGroup_EnemyCreatureBase_IsRequireDirectAwarenessOfThing()
{
    return true;
}
int main()
{
    if (CAIStateGroup_EnemyCreatureBase_IsRequireDirectAwarenessOfThing() == true) { std::printf("AUTO_TINY_008fe102_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fe102_TEST FAIL\n");
    return 1;
}