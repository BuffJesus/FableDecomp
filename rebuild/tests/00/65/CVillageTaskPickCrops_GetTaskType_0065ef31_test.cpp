#include <cstdio>
int __fastcall CVillageTaskPickCrops_GetTaskType()
{
    return 2;
}
int main()
{
    if (CVillageTaskPickCrops_GetTaskType() == 2) { std::printf("AUTO_TINY_0065ef31_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0065ef31_TEST FAIL\n");
    return 1;
}