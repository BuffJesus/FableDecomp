// CEngineScreenEffectRadialBlurRenderer::GetConsoleEnableFunctionName
// Retail 0x00b5ead0 : returns a CCharString built from a string literal + (-1).
struct CCharString {
    char* p;
    CCharString(const char* s, long len);   // out-of-line ctor -> emits a call
};

struct CEngineScreenEffectRadialBlurRenderer {
    void* vtbl;
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineScreenEffectRadialBlurRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("ENGINE_SCREEN_EFFECT_RADIAL_BLUR", -1);
}