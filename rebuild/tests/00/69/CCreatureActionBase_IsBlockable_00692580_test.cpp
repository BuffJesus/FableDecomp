#include <cstdio>
bool __fastcall CCreatureActionBase_IsBlockable()
{
    return true;
}
int main()
{
    if (CCreatureActionBase_IsBlockable() == true) { std::printf("AUTO_TINY_00692580_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692580_TEST FAIL\n");
    return 1;
}