#include <cstdio>
bool __fastcall CCreatureActionBase_IsPlayHitResponse()
{
    return true;
}
int main()
{
    if (CCreatureActionBase_IsPlayHitResponse() == true) { std::printf("AUTO_TINY_00692660_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692660_TEST FAIL\n");
    return 1;
}