#include <cstdio>
bool __fastcall CCreatureAction_ReleaseDivineWrathSpell_IsPlayHitResponse()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleaseDivineWrathSpell_IsPlayHitResponse() == false) { std::printf("AUTO_TINY_008bdbc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdbc0_TEST FAIL\n");
    return 1;
}