#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenLashOutAttack_IsBlockable()
{
    return true;
}
int main()
{
    if (CCreatureAction_WaspQueenLashOutAttack_IsBlockable() == true) { std::printf("AUTO_TINY_0095cb40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095cb40_TEST FAIL\n");
    return 1;
}