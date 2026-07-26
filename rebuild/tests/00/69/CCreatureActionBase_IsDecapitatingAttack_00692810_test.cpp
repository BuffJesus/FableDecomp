#include <cstdio>
bool __fastcall CCreatureActionBase_IsDecapitatingAttack()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsDecapitatingAttack() == false) { std::printf("AUTO_TINY_00692810_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692810_TEST FAIL\n");
    return 1;
}