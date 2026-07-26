#include <cstdio>
int __fastcall CAIStateGroup_Gaze_GetOwner()
{
    return 0;
}
int main()
{
    if (CAIStateGroup_Gaze_GetOwner() == 0) { std::printf("AUTO_TINY_00918ec0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00918ec0_TEST FAIL\n");
    return 1;
}