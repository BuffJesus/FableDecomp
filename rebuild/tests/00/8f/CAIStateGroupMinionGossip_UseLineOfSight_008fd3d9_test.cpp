#include <cstdio>
bool __fastcall CAIStateGroup_MinionGossip_UseLineOfSight()
{
    return true;
}
int main()
{
    if (CAIStateGroup_MinionGossip_UseLineOfSight() == true) { std::printf("AUTO_TINY_008fd3d9_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fd3d9_TEST FAIL\n");
    return 1;
}