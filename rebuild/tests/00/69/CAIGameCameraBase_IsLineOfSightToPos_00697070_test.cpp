#include <cstdio>
bool __fastcall CAIGameCameraBase_IsLineOfSightToPos(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CAIGameCameraBase_IsLineOfSightToPos(&object, 1, 2)) { std::printf("AUTO_TINY_00697070_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00697070_TEST FAIL\n");
    return 1;
}