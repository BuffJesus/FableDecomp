#include <cstdio>
bool __fastcall CCreatureAction_BlowHorn_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_BlowHorn_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_0084b220_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0084b220_TEST FAIL\n");
    return 1;
}