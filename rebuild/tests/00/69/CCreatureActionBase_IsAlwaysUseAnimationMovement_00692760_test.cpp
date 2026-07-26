#include <cstdio>
bool __fastcall CCreatureActionBase_IsAlwaysUseAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsAlwaysUseAnimationMovement() == false) { std::printf("AUTO_TINY_00692760_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692760_TEST FAIL\n");
    return 1;
}