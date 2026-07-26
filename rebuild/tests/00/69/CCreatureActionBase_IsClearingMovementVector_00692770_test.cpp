#include <cstdio>
bool __fastcall CCreatureActionBase_IsClearingMovementVector()
{
    return true;
}
int main()
{
    if (CCreatureActionBase_IsClearingMovementVector() == true) { std::printf("AUTO_TINY_00692770_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692770_TEST FAIL\n");
    return 1;
}