#include <cstdio>
int __fastcall CCreatureActionBase_GetAnimationTransitionOutTime()
{
    return 0;
}
int main()
{
    if (CCreatureActionBase_GetAnimationTransitionOutTime() == 0) { std::printf("AUTO_TINY_006924c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006924c0_TEST FAIL\n");
    return 1;
}