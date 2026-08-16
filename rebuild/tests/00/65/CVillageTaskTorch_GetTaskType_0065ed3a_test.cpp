#include <cstdio>
int __fastcall CVillageTaskTorch_GetTaskType()
{
    return 0;
}
int main()
{
    if (CVillageTaskTorch_GetTaskType() == 0) { std::printf("AUTO_TINY_0065ed3a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0065ed3a_TEST FAIL\n");
    return 1;
}