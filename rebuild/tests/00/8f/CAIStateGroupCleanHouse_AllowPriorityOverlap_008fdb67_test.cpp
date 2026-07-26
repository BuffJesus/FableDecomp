#include <cstdio>
bool __fastcall CAIStateGroup_CleanHouse_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_CleanHouse_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_008fdb67_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fdb67_TEST FAIL\n");
    return 1;
}