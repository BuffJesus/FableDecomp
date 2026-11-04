#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "fable_render_state_manager.h"
#include "fable_shader_render_manager.h"
#include "engine/CEngineScreenEffectColourFilter.h"
void CEngine::RenderColorFilteredQuad(float x,float y,const C4DVector& colour,float width,float height,CTexture* texture)
{
    FableCurrentRenderStateManager_013cb508->StartCaptureBlock();
    SetDefaultRenderStates();
    long previous=FableShaderRenderManager_013bc470.CurrentVSConstantLayoutIndex;
    FableShaderRenderManager_013bc470.SetVertexShaderConstantLayout(3);
    NEngineInternal::GPScreenEffectColourFilter->RenderColorFilteredQuad(x,y,colour,width,height,texture);
    FableShaderRenderManager_013bc470.SetVertexShaderConstantLayout(previous);
    FableCurrentRenderStateManager_013cb508->RestoreCaptureBlock();
}
