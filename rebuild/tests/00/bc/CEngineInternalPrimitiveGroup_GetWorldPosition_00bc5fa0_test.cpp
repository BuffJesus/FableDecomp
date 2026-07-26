#include <cstdio>
bool __fastcall CEngineInternalPrimitiveGroup_GetWorldPosition(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CEngineInternalPrimitiveGroup_GetWorldPosition(&object, 1, 2)) { std::printf("AUTO_TINY_00bc5fa0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00bc5fa0_TEST FAIL\n");
    return 1;
}