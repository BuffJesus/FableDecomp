#include <cstdio>
int __fastcall CVillageTaskSecuritySweep_GetTaskType()
{
    return 3;
}
int main()
{
    if (CVillageTaskSecuritySweep_GetTaskType() == 3) { std::printf("AUTO_TINY_008602f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008602f0_TEST FAIL\n");
    return 1;
}