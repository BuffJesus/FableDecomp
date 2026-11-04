#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineScene.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "fable_engine_component_params.h"
#include "fable_engine_internal.h"
#include "fable_render_state_manager.h"
using namespace NEngineInternal;
void CEnginePrimitiveRenderer::Render(const CEngineComponentRenderParams& params)
{
    if(!(params.Params->SceneFilterFlags&1))return;
    FableCurrentRenderStateManager_013cb508->StartCaptureBlock();
    PreRenderLayer(params.RenderLayer);
    switch(params.RenderLayer){
    case RENDER_LAYER_NON_SORTED_PRIMITIVES:
        RenderNonsortedPrimitives();break;
    case RENDER_LAYER_SHADOW_BUFFER_RENDER:
        if(GPShadowRenderer->RenderEnabled&&GPShadowRenderer->RenderingTechnique!=CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_NO_SHADOWS)
            for(CEngineScene** i=SceneList.begin();i!=SceneList.end();++i)
                (*i)->RenderSceneShadowBuffer(params.RenderLayer,GPShadowRenderer->PeekSceneData().CasterClipVolume);
        break;
    case RENDER_LAYER_OUTLINE_FILL:
        if(GPScreenEffectOutlineGlow->RenderEnabled){
            GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_OUTLINE_FILL,true);
            if(GPSubPrimitiveRenderer->GetCount(SUBPRIM_TYPE_OUTLINE_SMOOTH_FILL)>0){
                GPScreenEffectOutlineGlow->BeginOutlineRendering();
                GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_OUTLINE_SMOOTH_FILL,true);
                GPScreenEffectOutlineGlow->FinishOutlineRendering();
            }
        }else{
            GPSubPrimitiveRenderer->ClearType(SUBPRIM_TYPE_OUTLINE_FILL);
            GPSubPrimitiveRenderer->ClearType(SUBPRIM_TYPE_OUTLINE_SMOOTH_FILL);
        }
        break;
    case RENDER_LAYER_OUTLINE_ALPHA_STENCIL_MASK:
        if(GPScreenEffectOutlineGlow->RenderEnabled)GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_OUTLINE_ALPHA_STENCIL_MASK_SMOOTH,true);
        else GPSubPrimitiveRenderer->ClearType(SUBPRIM_TYPE_OUTLINE_ALPHA_STENCIL_MASK_SMOOTH);
        break;
    case RENDER_LAYER_OUTLINE_STENCIL:
        if(GPScreenEffectOutlineGlow->RenderEnabled){
            GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_OUTLINE_STENCIL_SMOOTH,true);
            GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_OUTLINE_STENCIL,true);
        }else{
            GPSubPrimitiveRenderer->ClearType(SUBPRIM_TYPE_OUTLINE_STENCIL_SMOOTH);
            GPSubPrimitiveRenderer->ClearType(SUBPRIM_TYPE_OUTLINE_STENCIL);
        }
        break;
    case RENDER_LAYER_STENCIL_MESHES:GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_STENCIL_MESHES,true);break;
    case RENDER_LAYER_POSTWATER_MESHES:GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_POSTWATER_MESHES,true);break;
    case RENDER_LAYER_BOUNDING_BOXES:GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_BOUNDING_VOLUME,true);break;
    case RENDER_LAYER_DISPLACEMENT_BUFFER:
        if(GPSubPrimitiveRenderer->GetCount(SUBPRIM_TYPE_DISPLACEMENT_BUFFER)>0){
            if(GPScreenEffectDisplacementRenderer->BeginDisplacementRendering()){
                GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_DISPLACEMENT_BUFFER,true);
                GPScreenEffectDisplacementRenderer->FinishDisplacementRendering();
            }
            GPSubPrimitiveRenderer->ClearType(SUBPRIM_TYPE_DISPLACEMENT_BUFFER);
        }
        break;
    case RENDER_LAYER_DISPLACEMENT_STENCIL:
        if(GPSubPrimitiveRenderer->GetCount(SUBPRIM_TYPE_DISPLACEMENT_STENCIL)>0){
            GPScreenEffectDisplacementRenderer->BeginDisplacementStencilRendering();
            GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_DISPLACEMENT_STENCIL,true);
            GPScreenEffectDisplacementRenderer->FinishDisplacementStencilRendering();
            GPSubPrimitiveRenderer->ClearType(SUBPRIM_TYPE_DISPLACEMENT_STENCIL);
        }
        break;
    default:
        for(CEngineScene** i=SceneList.begin();i!=SceneList.end();++i){
            if((params.RenderLayerFlags&RENDER_LAYER_FLAG_USE_SCENE_OCCLUSION)&&OcclusionEnable)
                (*i)->RenderSceneAndCheckVisibility(params.RenderLayer,*GPCamera,CurrentOcclusionLayer,!OcclusionHierarchyEnable);
            else (*i)->RenderScene(params.RenderLayer,*GPCamera);
        }
        break;
    }
    GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_OPAQUE,true);
    if(params.RenderLayerFlags&RENDER_LAYER_FLAG_FLUSH_ALPHA_PRIMITIVES)GPSubPrimitiveRenderer->RenderType(SUBPRIM_TYPE_ALPHA,true);
    PostRenderLayer();
    FableCurrentRenderStateManager_013cb508->RestoreCaptureBlock();
}
