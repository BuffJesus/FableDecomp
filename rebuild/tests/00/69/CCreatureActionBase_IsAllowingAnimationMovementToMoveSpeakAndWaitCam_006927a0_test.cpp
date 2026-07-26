#include <cstdio>
bool __fastcall CCreatureActionBase_IsAllowingAnimationMovementToMoveSpeakAndWaitCameras()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() == false) { std::printf("AUTO_TINY_006927a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006927a0_TEST FAIL\n");
    return 1;
}