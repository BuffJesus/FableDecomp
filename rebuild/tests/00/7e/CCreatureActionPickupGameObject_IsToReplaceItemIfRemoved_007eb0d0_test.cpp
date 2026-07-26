#include <cstdio>
bool __fastcall CCreatureAction_PickupGameObject_IsToReplaceItemIfRemoved()
{
    return true;
}
int main()
{
    if (CCreatureAction_PickupGameObject_IsToReplaceItemIfRemoved() == true) { std::printf("AUTO_TINY_007eb0d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007eb0d0_TEST FAIL\n");
    return 1;
}