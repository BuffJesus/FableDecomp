#include <cstdio>
bool __fastcall CCreatureAction_NymphShot_IsStrikeAction()
{
    return true;
}
int main()
{
    if (CCreatureAction_NymphShot_IsStrikeAction() == true) { std::printf("AUTO_TINY_008ca900_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ca900_TEST FAIL\n");
    return 1;
}