#include <cstdio>
bool __fastcall CAIGameCameraBase_IsFreeLookMode()
{
    return false;
}
int main()
{
    if (CAIGameCameraBase_IsFreeLookMode() == false) { std::printf("AUTO_TINY_006970d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006970d0_TEST FAIL\n");
    return 1;
}