#include <cstdio>
int __fastcall CAIStateGroup_GazeOutside_GetOwner()
{
    return 0;
}
int main()
{
    if (CAIStateGroup_GazeOutside_GetOwner() == 0) { std::printf("AUTO_TINY_00918f10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00918f10_TEST FAIL\n");
    return 1;
}