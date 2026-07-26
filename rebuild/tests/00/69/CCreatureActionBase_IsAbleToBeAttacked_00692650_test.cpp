#include <cstdio>
bool __fastcall CCreatureActionBase_IsAbleToBeAttacked()
{
    return true;
}
int main()
{
    if (CCreatureActionBase_IsAbleToBeAttacked() == true) { std::printf("AUTO_TINY_00692650_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692650_TEST FAIL\n");
    return 1;
}