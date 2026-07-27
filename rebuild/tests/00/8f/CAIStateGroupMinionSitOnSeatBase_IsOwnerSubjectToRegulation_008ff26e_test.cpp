#include <cstdio>
bool __fastcall CAIStateGroup_MinionSitOnSeatBase_IsOwnerSubjectToRegulation()
{
    return false;
}
int main()
{
    if (CAIStateGroup_MinionSitOnSeatBase_IsOwnerSubjectToRegulation() == false) { std::printf("AUTO_TINY_008ff26e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ff26e_TEST FAIL\n");
    return 1;
}