#include <cstdio>
bool __fastcall CCreatureAction_BriarRoseConcentrateInto_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_BriarRoseConcentrateInto_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008e8c10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008e8c10_TEST FAIL\n");
    return 1;
}