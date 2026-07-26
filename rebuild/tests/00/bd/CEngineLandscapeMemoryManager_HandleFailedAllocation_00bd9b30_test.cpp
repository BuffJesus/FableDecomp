#include <cstdio>
bool __fastcall CEngineLandscapeMemoryManager_HandleFailedAllocation(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CEngineLandscapeMemoryManager_HandleFailedAllocation(&object, 1, 2)) { std::printf("AUTO_TINY_00bd9b30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00bd9b30_TEST FAIL\n");
    return 1;
}