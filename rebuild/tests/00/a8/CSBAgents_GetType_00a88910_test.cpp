#include <cstdio>
int __fastcall CSBAgents_GetType()
{
    return 0;
}
int main()
{
    if (CSBAgents_GetType() == 0) { std::printf("AUTO_TINY_00a88910_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a88910_TEST FAIL\n");
    return 1;
}