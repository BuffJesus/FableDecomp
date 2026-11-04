#include "engine/CEngineScreenEffectRadialBlurRenderer.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "fable_engine_internal.h"
namespace {
inline void FreePage(long page) {
    if(page>=0)reinterpret_cast<CGPUScratchBufferTextureManager*>(NEngineInternal::GPResourceManager->GPUScratchBufferTextureManager)->FreeMemoryPage(page);
}
}
bool CEngineScreenEffectRadialBlurRenderer::SetEffectQuality(const CEngineVideoOptionsDef& quality)
{
    if(quality.EnableRadialBlur && EffectSupported) {
        if(InitialiseTextures()) {RenderEnabled=true;return true;}
        FreePage(DynamicTexturePageHandle);
        RenderEnabled=false;
        return false;
    }
    FreePage(DynamicTexturePageHandle);
    RenderEnabled=false;
    return true;
}
void CEngineScreenEffectRadialBlurRenderer::OnReleaseDefaultPoolResources()
{
    DynamicTexturePageHandle=-1;
}
