#include <cstdio>

unsigned char g_movieSeqCameraFixed;
struct CGameScriptInterface;

void __fastcall CGameScriptInterface_FixMovieSequenceCamera(CGameScriptInterface* self, void* edx, unsigned char enabled)
{
    (void)self;
    (void)edx;
    g_movieSeqCameraFixed = (unsigned char)(enabled == 0);
}

int main()
{
    CGameScriptInterface_FixMovieSequenceCamera((CGameScriptInterface*)0, 0, 0);
    if (g_movieSeqCameraFixed != 1) { std::printf("FAIL zero-case\n"); return 1; }
    CGameScriptInterface_FixMovieSequenceCamera((CGameScriptInterface*)0, 0, 5);
    if (g_movieSeqCameraFixed != 0) { std::printf("FAIL nonzero-case\n"); return 1; }
    std::printf("CGameScriptInterface_0088e500_TEST PASS\n");
    return 0;
}