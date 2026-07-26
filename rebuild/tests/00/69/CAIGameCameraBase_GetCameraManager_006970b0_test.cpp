#include <cstdio>
bool __fastcall CAIGameCameraBase_GetCameraManager(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CAIGameCameraBase_GetCameraManager(&object, 1, 2)) { std::printf("AUTO_TINY_006970b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006970b0_TEST FAIL\n");
    return 1;
}