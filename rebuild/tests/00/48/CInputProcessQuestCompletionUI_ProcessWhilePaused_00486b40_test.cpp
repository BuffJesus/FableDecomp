#include <cstdio>
bool __fastcall CInputProcessQuestCompletionUI_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessQuestCompletionUI_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_00486b40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00486b40_TEST FAIL\n");
    return 1;
}