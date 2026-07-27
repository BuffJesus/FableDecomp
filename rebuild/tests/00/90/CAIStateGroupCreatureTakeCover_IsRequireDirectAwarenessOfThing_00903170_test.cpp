#include <cstdio>
bool __fastcall CAIStateGroup_CreatureTakeCover_IsRequireDirectAwarenessOfThing()
{
    return false;
}
int main()
{
    if (CAIStateGroup_CreatureTakeCover_IsRequireDirectAwarenessOfThing() == false) { std::printf("AUTO_TINY_00903170_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00903170_TEST FAIL\n");
    return 1;
}