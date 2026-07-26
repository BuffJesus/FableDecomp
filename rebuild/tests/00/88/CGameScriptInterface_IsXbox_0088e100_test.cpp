#include <cstdio>
bool __fastcall CGameScriptInterface_IsXbox()
{
    return false;
}
int main()
{
    if (CGameScriptInterface_IsXbox() == false) { std::printf("AUTO_TINY_0088e100_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0088e100_TEST FAIL\n");
    return 1;
}