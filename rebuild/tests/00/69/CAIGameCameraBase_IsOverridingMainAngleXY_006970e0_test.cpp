#include <cstdio>
bool __fastcall CAIGameCameraBase_IsOverridingMainAngleXY()
{
    return false;
}
int main()
{
    if (CAIGameCameraBase_IsOverridingMainAngleXY() == false) { std::printf("AUTO_TINY_006970e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006970e0_TEST FAIL\n");
    return 1;
}