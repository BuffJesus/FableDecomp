#include <cstdio>
bool __fastcall CCreatureAction_PickupGameObject_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_PickupGameObject_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_007eb0c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007eb0c0_TEST FAIL\n");
    return 1;
}