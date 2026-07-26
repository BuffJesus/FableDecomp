#include <cstdio>
void __stdcall CGameScriptInterface_Error(int arg0, int arg1, int arg2, int arg3) {}
int main()
{
    CGameScriptInterface_Error(1, 2, 3, 4);
    std::printf("AUTO_TINY_0088de00_TEST PASS\n");
    return 0;
}