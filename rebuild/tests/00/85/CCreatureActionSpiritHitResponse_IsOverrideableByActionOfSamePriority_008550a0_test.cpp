#include <cstdio>
bool __fastcall CCreatureAction_SpiritHitResponse_IsOverrideableByActionOfSamePriority()
{
    return true;
}
int main()
{
    if (CCreatureAction_SpiritHitResponse_IsOverrideableByActionOfSamePriority() == true) { std::printf("AUTO_TINY_008550a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008550a0_TEST FAIL\n");
    return 1;
}