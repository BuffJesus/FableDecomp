#include <cstdio>
bool __fastcall CCreatureAction_BlockCounter_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_BlockCounter_IsBlockable() == false) { std::printf("AUTO_TINY_00855d70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00855d70_TEST FAIL\n");
    return 1;
}