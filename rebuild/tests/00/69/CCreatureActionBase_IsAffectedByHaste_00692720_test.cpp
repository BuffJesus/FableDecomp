#include <cstdio>
bool __fastcall CCreatureActionBase_IsAffectedByHaste()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsAffectedByHaste() == false) { std::printf("AUTO_TINY_00692720_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692720_TEST FAIL\n");
    return 1;
}