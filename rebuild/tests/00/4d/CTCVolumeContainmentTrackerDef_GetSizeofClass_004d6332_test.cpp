#include <cstdio>
int __fastcall CTCVolumeContainmentTrackerDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CTCVolumeContainmentTrackerDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004d6332_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d6332_TEST FAIL\n");
    return 1;
}