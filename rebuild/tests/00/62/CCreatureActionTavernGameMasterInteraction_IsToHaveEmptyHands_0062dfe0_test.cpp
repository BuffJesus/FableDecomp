#include <cstdio>
bool __fastcall CCreatureAction_TavernGameMasterInteraction_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_TavernGameMasterInteraction_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_0062dfe0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062dfe0_TEST FAIL\n");
    return 1;
}