#include <cstdio>
bool __fastcall CCreatureActionHero180RunningTurn_CanAnimationTransitionOut()
{
    return true;
}
int main()
{
    if (CCreatureActionHero180RunningTurn_CanAnimationTransitionOut() == true) { std::printf("AUTO_TINY_0085b880_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0085b880_TEST FAIL\n");
    return 1;
}