#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "fable_engine_internal.h"
namespace {
inline void FreePage(long page) {
    if(page>=0)reinterpret_cast<CGPUScratchBufferTextureManager*>(NEngineInternal::GPResourceManager->GPUScratchBufferTextureManager)->FreeMemoryPage(page);
}
}
bool CEngineScreenEffectDisplacementRenderer::SetEffectQuality(const CEngineVideoOptionsDef& quality)
{
    Enable2DDisplacement=EffectSupported && quality.Enable2DDisplacement;
    Enable3DDisplacement=EffectSupported && quality.Enable3DDisplacement;
    if(!Enable2DDisplacement && !Enable3DDisplacement) {
        FreePage(DynamicTexturePageHandle);
        RenderEnabled=false;
        return true;
    }
    if(InitialiseTextures()) {RenderEnabled=true;return true;}
    FreePage(DynamicTexturePageHandle);
    RenderEnabled=false;
    return false;
}
void CEngineScreenEffectDisplacementRenderer::OnReleaseDefaultPoolResources()
{
    DynamicTexturePageHandle=-1;
}
