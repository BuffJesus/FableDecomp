#include <cstdio>
int __fastcall CVillageTaskCarryCrate_GetTaskType()
{
    return 5;
}
int main()
{
    if (CVillageTaskCarryCrate_GetTaskType() == 5) { std::printf("AUTO_TINY_0065efd4_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0065efd4_TEST FAIL\n");
    return 1;
}