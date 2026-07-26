#include <cstdio>
bool __fastcall CAIGameCameraBase_IsPosInViewingFrustrum(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CAIGameCameraBase_IsPosInViewingFrustrum(&object, 1, 2)) { std::printf("AUTO_TINY_00697040_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00697040_TEST FAIL\n");
    return 1;
}