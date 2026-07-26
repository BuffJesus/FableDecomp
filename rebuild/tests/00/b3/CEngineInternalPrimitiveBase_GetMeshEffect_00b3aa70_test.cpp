#include <cstdio>
bool __fastcall CEngineInternalPrimitiveBase_GetMeshEffect(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CEngineInternalPrimitiveBase_GetMeshEffect(&object, 1, 2)) { std::printf("AUTO_TINY_00b3aa70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b3aa70_TEST FAIL\n");
    return 1;
}