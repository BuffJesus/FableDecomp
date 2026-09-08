#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

struct CCharString { char* p; CCharString(const char*, long); };
struct CEngineScreenEffectRadialBlurRenderer_Methods : CEngineScreenEffectRadialBlurRenderer {
    CCharString GetConsoleEnableFunctionName();
};
CCharString CEngineScreenEffectRadialBlurRenderer_Methods::GetConsoleEnableFunctionName() {
    return CCharString("ENGINE_SCREEN_EFFECT_RADIAL_BLUR", -1);
}
