#include <cstdio>
bool __fastcall CCreatureActionBase_IsBlockCounter()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsBlockCounter() == false) { std::printf("AUTO_TINY_006926c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006926c0_TEST FAIL\n");
    return 1;
}