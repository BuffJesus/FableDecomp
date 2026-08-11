#include <stdio.h>
#include <string.h>

struct CCharString {
    char* p;
    CCharString(const char* s, long len);
};

static char g_buf[64];
CCharString::CCharString(const char* s, long len) {
    (void)len;
    if (s) { strncpy(g_buf, s, sizeof(g_buf)-1); g_buf[sizeof(g_buf)-1]=0; }
    else   { g_buf[0]=0; }
    p = g_buf;
}

struct CEngineScreenEffectRadialBlurRenderer {
    void* vtbl;
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineScreenEffectRadialBlurRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("ENGINE_SCREEN_EFFECT_RADIAL_BLUR", -1);
}

int main() {
    CEngineScreenEffectRadialBlurRenderer obj;
    obj.vtbl = 0;
    CCharString r = obj.GetConsoleEnableFunctionName();
    if (r.p && strcmp(r.p, "ENGINE_SCREEN_EFFECT_RADIAL_BLUR") == 0) {
        printf("RADIALBLUR_GETNAME_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}