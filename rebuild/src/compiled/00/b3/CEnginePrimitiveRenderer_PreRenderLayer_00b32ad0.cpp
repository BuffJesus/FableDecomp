#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineLightingManager.h"
#include "fable_engine_internal.h"
#include "fable_engine_layer_renderer.h"
#include "fable_shader_render_manager.h"
#include "fable_display_viewport.h"
using namespace NEngineInternal;
void CEnginePrimitiveRenderer::PreRenderLayer(EEngineRenderLayer layer)
{
    OldVertexShadersEnable=GPShaderRenderManager->VertexShadersEnabled;
    OldPixelShadersEnable=GPShaderRenderManager->PixelShadersEnabled;
    if(layer==RENDER_LAYER_SHADOW_BUFFER_RENDER)return;
    CShaderRenderManager* shader=GPShaderRenderManager;
    if(shader->VertexShadersEnabled!=true){shader->EnableVertexShaders();shader=GPShaderRenderManager;}
    if(layer==RENDER_LAYER_BOUNDING_BOXES){
        if(shader->PixelShadersEnabled!=false){shader->DisablePixelShaders();shader=GPShaderRenderManager;}
        shader->ResetWorldTransform();
        GPRenderManager->SetTextureOperation(0,TEXTURE_OP_DISABLE);
        GPRenderManager->SetTextureAlphaOperation(0,TEXTURE_OP_DISABLE);
        GPRenderManager->SetColourWriteEnable(false);
        GPRenderManager->SetFogEnable(false);
        GPRenderManager->SetZBufferWriteEnable(false);
        GPRenderManager->SetAlphaTestEnable(false);
        GPRenderManager->SetAlphaBlendEnable(false);
    }else{
        if(shader->PixelShadersEnabled!=true)shader->EnablePixelShaders();
        if(layer==RENDER_LAYER_MESHES_OCCLUSION||layer==RENDER_LAYER_MESHES_NO_OCCLUSION||layer==RENDER_LAYER_NON_SORTED_PRIMITIVES||layer==RENDER_LAYER_STENCIL_MESHES||layer==RENDER_LAYER_POSTWATER_MESHES||layer==RENDER_LAYER_LANDSCAPE_BASE||layer==RENDER_LAYER_LANDSCAPE_POST_SOLID_SCENE)
            GPLightingManager->EnableFog();
    }
    InitialiseGlobalLights();
    CurrentOcclusionLayer=static_cast<EEngineOcclusionLayer>(OcclusionEnable?0:-1);
}
