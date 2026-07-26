#include <cstdio>
bool __fastcall CCreatureAction_QueenHornetShootBabies_IsStrikeAction()
{
    return true;
}
int main()
{
    if (CCreatureAction_QueenHornetShootBabies_IsStrikeAction() == true) { std::printf("AUTO_TINY_0095cc00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095cc00_TEST FAIL\n");
    return 1;
}