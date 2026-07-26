#include <cstdio>

void __fastcall CGameScriptInterface_DisplayTime(void* self, int edx_unused, int arg)
{
    (void)self;
    (void)edx_unused;
    (void)arg;
}

int main()
{
    int marker = 12345;
    CGameScriptInterface_DisplayTime(&marker, 0, 99);
    if (marker != 12345) { std::printf("CGameScriptInterface_0088e210_TEST FAIL\n"); return 1; }
    std::printf("CGameScriptInterface_0088e210_TEST PASS\n");
    return 0;
}