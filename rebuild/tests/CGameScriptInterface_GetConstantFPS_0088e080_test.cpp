#include <cstdio>

struct CGameScriptInterface;
long g_constantFPS = 0;

long __fastcall CGameScriptInterface_GetConstantFPS(const CGameScriptInterface* self)
{
    (void)self;
    return g_constantFPS;
}

int main()
{
    g_constantFPS = 60;
    long r = CGameScriptInterface_GetConstantFPS((const CGameScriptInterface*)0);
    if (r != 60) { std::printf("FAIL got %ld\n", r); return 1; }
    g_constantFPS = 30;
    r = CGameScriptInterface_GetConstantFPS((const CGameScriptInterface*)0);
    if (r != 30) { std::printf("FAIL got %ld\n", r); return 1; }
    std::printf("CGameScriptInterface_0088e080_TEST PASS\n");
    return 0;
}