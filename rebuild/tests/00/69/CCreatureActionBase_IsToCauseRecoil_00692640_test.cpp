#include <cstdio>
bool __fastcall CCreatureActionBase_IsToCauseRecoil()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsToCauseRecoil() == false) { std::printf("AUTO_TINY_00692640_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692640_TEST FAIL\n");
    return 1;
}