#include <cstdio>
bool __fastcall CCreatureAction_RiseUp_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_RiseUp_IsBlockable() == false) { std::printf("AUTO_TINY_0074db50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0074db50_TEST FAIL\n");
    return 1;
}