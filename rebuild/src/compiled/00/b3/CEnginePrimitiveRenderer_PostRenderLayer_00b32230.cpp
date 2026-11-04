#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineLightingManager.h"
#include "fable_engine_internal.h"
#include "fable_shader_render_manager.h"
#include "fable_display_viewport.h"
void CEnginePrimitiveRenderer::PostRenderLayer()
{
    CurrentOcclusionLayer=static_cast<EEngineOcclusionLayer>(-1);
    NEngineInternal::GPRenderManager->SetNormalizeNormals(false);
    NEngineInternal::GPLightingManager->DisablePointLights();
    ResetWorldTransform();
    CShaderRenderManager* shader=NEngineInternal::GPShaderRenderManager;
    if(OldPixelShadersEnable!=shader->PixelShadersEnabled){
        if(OldPixelShadersEnable)shader->EnablePixelShaders();else shader->DisablePixelShaders();
        shader=NEngineInternal::GPShaderRenderManager;
    }
    if(OldVertexShadersEnable!=shader->VertexShadersEnabled){
        if(OldVertexShadersEnable)shader->EnableVertexShaders();else shader->DisableVertexShaders();
    }
}
