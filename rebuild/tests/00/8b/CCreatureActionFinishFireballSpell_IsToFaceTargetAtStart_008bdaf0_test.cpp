#include <cstdio>
bool __fastcall CCreatureAction_FinishFireballSpell_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_FinishFireballSpell_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008bdaf0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdaf0_TEST FAIL\n");
    return 1;
}