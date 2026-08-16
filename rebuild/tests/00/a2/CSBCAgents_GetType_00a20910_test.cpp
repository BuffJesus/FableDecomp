#include <cstdio>
int __fastcall CSBCAgents_GetType()
{
    return 4;
}
int main()
{
    if (CSBCAgents_GetType() == 4) { std::printf("AUTO_TINY_00a20910_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a20910_TEST FAIL\n");
    return 1;
}