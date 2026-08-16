#include <cstdio>
int __fastcall CSBIAgents_GetType()
{
    return 2;
}
int main()
{
    if (CSBIAgents_GetType() == 2) { std::printf("AUTO_TINY_00a211d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a211d0_TEST FAIL\n");
    return 1;
}