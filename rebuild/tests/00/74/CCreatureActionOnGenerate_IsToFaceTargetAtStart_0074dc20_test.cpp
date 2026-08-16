#include <cstdio>
bool __fastcall CCreatureAction_OnGenerate_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_OnGenerate_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_0074dc20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0074dc20_TEST FAIL\n");
    return 1;
}