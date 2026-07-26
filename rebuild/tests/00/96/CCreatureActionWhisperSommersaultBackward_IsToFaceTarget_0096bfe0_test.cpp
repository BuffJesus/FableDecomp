#include <cstdio>
bool __fastcall CCreatureAction_WhisperSommersaultBackward_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_WhisperSommersaultBackward_IsToFaceTarget() == true) { std::printf("AUTO_TINY_0096bfe0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0096bfe0_TEST FAIL\n");
    return 1;
}