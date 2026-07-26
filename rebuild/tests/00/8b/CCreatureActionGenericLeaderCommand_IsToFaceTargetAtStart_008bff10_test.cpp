#include <cstdio>
bool __fastcall CCreatureAction_GenericLeaderCommand_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_GenericLeaderCommand_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008bff10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff10_TEST FAIL\n");
    return 1;
}