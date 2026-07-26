#include <cstdio>
bool __fastcall CCreatureActionBase_IsQueuable()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsQueuable() == false) { std::printf("AUTO_TINY_00692560_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692560_TEST FAIL\n");
    return 1;
}