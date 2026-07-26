#include <cstdio>
bool __fastcall CAIStateGroup_MinionBehaviourPodiumEmoteNeutral_IsRunnableLeaf()
{
    return true;
}
int main()
{
    if (CAIStateGroup_MinionBehaviourPodiumEmoteNeutral_IsRunnableLeaf() == true) { std::printf("AUTO_TINY_0096f4a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0096f4a0_TEST FAIL\n");
    return 1;
}