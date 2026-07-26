#include <cstdio>
bool __fastcall CCreatureAction_NymphTeleportLimboBase_IsMoving()
{
    return false;
}
int main()
{
    if (CCreatureAction_NymphTeleportLimboBase_IsMoving() == false) { std::printf("AUTO_TINY_008ca920_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ca920_TEST FAIL\n");
    return 1;
}