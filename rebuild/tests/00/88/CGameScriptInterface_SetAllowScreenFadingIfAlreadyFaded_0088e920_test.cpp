#include <cstdio>

struct CGameScriptInterface {
    void* vt; // +0x0
    unsigned char _pad[0x29]; // to 0x2d
    char allowFadeIfFaded; // +0x2d
};

void __fastcall CGameScriptInterface_SetAllowScreenFadingIfAlreadyFaded(CGameScriptInterface* self, int /*edx*/, char val)
{
    self->allowFadeIfFaded = val;
}

int main()
{
    CGameScriptInterface s;
    for (int i = 0; i < (int)sizeof(s); ++i) ((char*)&s)[i] = 0;
    CGameScriptInterface_SetAllowScreenFadingIfAlreadyFaded(&s, 0, 1);
    if (s.allowFadeIfFaded != 1) { std::printf("FAIL set 1\n"); return 1; }
    CGameScriptInterface_SetAllowScreenFadingIfAlreadyFaded(&s, 0, 0);
    if (s.allowFadeIfFaded != 0) { std::printf("FAIL set 0\n"); return 1; }
    std::printf("CGameScriptInterface_0088e920_TEST PASS\n");
    return 0;
}