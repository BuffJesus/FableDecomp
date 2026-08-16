#include <cstdio>
bool __fastcall CCreatureAction_QueenHornetShootBabies_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_QueenHornetShootBabies_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_0095cc10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095cc10_TEST FAIL\n");
    return 1;
}