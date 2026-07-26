#include <cstdio>
bool __fastcall CCreatureAction_WhisperSommersaultForward_IsToFaceTarget()
{
    return false;
}
int main()
{
    if (CCreatureAction_WhisperSommersaultForward_IsToFaceTarget() == false) { std::printf("AUTO_TINY_0096bf60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0096bf60_TEST FAIL\n");
    return 1;
}