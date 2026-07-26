#include <cstdio>
bool __fastcall CAIStateGroup_MinionSitAtInn_IsOwnerSubjectToRegulation()
{
    return true;
}
int main()
{
    if (CAIStateGroup_MinionSitAtInn_IsOwnerSubjectToRegulation() == true) { std::printf("AUTO_TINY_008ff2cd_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ff2cd_TEST FAIL\n");
    return 1;
}