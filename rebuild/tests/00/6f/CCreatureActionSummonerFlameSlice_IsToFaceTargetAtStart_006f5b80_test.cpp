#include <cstdio>
bool __fastcall CCreatureAction_SummonerFlameSlice_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_SummonerFlameSlice_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_006f5b80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5b80_TEST FAIL\n");
    return 1;
}