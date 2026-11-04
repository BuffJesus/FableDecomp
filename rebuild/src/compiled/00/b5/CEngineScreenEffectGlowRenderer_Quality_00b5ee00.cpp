#include "engine/CEngineScreenEffectGlowRenderer.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "fable_engine_internal.h"

namespace {
inline void FreePage(long page) {
    if(page>=0)reinterpret_cast<CGPUScratchBufferTextureManager*>(NEngineInternal::GPResourceManager->GPUScratchBufferTextureManager)->FreeMemoryPage(page);
}
}
bool CEngineScreenEffectGlowRenderer::SetEffectQuality(const CEngineVideoOptionsDef& quality)
{
    if(quality.EnableGlow && EffectSupported) {
        if(InitialiseTextures()) {RenderEnabled=true;return true;}
        FreePage(DynamicTexturePageHandle);
        RenderEnabled=false;
        return false;
    }
    RenderEnabled=false;
    FreePage(DynamicTexturePageHandle);
    return true;
}
void CEngineScreenEffectGlowRenderer::OnReleaseDefaultPoolResources()
{
    DynamicTexturePageHandle=-1;
}
