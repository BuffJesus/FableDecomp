#include <cstdio>
bool __fastcall CCreatureAction_UndeadRiseUp_IsBlockable()
{
    return true;
}
int main()
{
    if (CCreatureAction_UndeadRiseUp_IsBlockable() == true) { std::printf("AUTO_TINY_0074dcd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0074dcd0_TEST FAIL\n");
    return 1;
}