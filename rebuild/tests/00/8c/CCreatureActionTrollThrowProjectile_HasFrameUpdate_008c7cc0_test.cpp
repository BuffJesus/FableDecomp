#include <cstdio>
bool __fastcall CCreatureAction_TrollThrowProjectile_HasFrameUpdate()
{
    return true;
}
int main()
{
    if (CCreatureAction_TrollThrowProjectile_HasFrameUpdate() == true) { std::printf("AUTO_TINY_008c7cc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c7cc0_TEST FAIL\n");
    return 1;
}