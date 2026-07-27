#include <cstdio>
bool __fastcall CCreatureActionBase_IsToHaveEmptyHands()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsToHaveEmptyHands() == false) { std::printf("AUTO_TINY_00692780_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692780_TEST FAIL\n");
    return 1;
}