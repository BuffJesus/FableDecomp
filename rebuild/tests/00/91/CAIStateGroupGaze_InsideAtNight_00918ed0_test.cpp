#include <cstdio>
bool __fastcall CAIStateGroup_Gaze_InsideAtNight()
{
    return false;
}
int main()
{
    if (CAIStateGroup_Gaze_InsideAtNight() == false) { std::printf("AUTO_TINY_00918ed0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00918ed0_TEST FAIL\n");
    return 1;
}