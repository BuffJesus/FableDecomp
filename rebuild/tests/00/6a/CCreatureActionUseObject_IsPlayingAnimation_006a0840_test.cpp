#include <cstdio>
bool __fastcall CCreatureAction_UseObject_IsPlayingAnimation()
{
    return false;
}
int main()
{
    if (CCreatureAction_UseObject_IsPlayingAnimation() == false) { std::printf("AUTO_TINY_006a0840_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a0840_TEST FAIL\n");
    return 1;
}