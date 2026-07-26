#include <cstdio>
bool __fastcall CAIStateGroup_CreatureAdvanceInCover_IsRequireDirectAwarenessOfThing()
{
    return false;
}
int main()
{
    if (CAIStateGroup_CreatureAdvanceInCover_IsRequireDirectAwarenessOfThing() == false) { std::printf("AUTO_TINY_00903158_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00903158_TEST FAIL\n");
    return 1;
}