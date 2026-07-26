#include <cstdio>
bool __fastcall CCreatureActionBase_IsPhysicsPushableByHero()
{
    return true;
}
int main()
{
    if (CCreatureActionBase_IsPhysicsPushableByHero() == true) { std::printf("AUTO_TINY_00692740_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692740_TEST FAIL\n");
    return 1;
}