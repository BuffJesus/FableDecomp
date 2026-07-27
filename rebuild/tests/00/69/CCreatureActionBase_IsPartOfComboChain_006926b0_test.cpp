#include <cstdio>
bool __fastcall CCreatureActionBase_IsPartOfComboChain()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsPartOfComboChain() == false) { std::printf("AUTO_TINY_006926b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006926b0_TEST FAIL\n");
    return 1;
}