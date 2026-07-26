#include <cstdio>
bool __fastcall CCreatureAction_PickUpGenericBox_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_PickUpGenericBox_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_0084b3a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0084b3a0_TEST FAIL\n");
    return 1;
}