#include <cstdio>
int __fastcall CVillageTaskHorn_GetTaskType()
{
    return 4;
}
int main()
{
    if (CVillageTaskHorn_GetTaskType() == 4) { std::printf("AUTO_TINY_0065edec_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0065edec_TEST FAIL\n");
    return 1;
}