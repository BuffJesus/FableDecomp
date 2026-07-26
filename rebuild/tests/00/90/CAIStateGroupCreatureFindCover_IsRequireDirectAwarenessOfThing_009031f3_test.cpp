#include <cstdio>
bool __fastcall CAIStateGroup_CreatureFindCover_IsRequireDirectAwarenessOfThing()
{
    return false;
}
int main()
{
    if (CAIStateGroup_CreatureFindCover_IsRequireDirectAwarenessOfThing() == false) { std::printf("AUTO_TINY_009031f3_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009031f3_TEST FAIL\n");
    return 1;
}