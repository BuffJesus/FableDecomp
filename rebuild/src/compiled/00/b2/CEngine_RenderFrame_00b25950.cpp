#include "fable_engine.h"
#include "fable_engine_component_params.h"
#include "fable_engine_component.h"
#include "fable_engine_layer_renderer.h"
#include "fable_render_state_manager.h"
void CEngine::RenderFrame(const CEngineInterpolationInfo& interpolation)
{
    FableCurrentRenderStateManager_013cb508->StartCaptureBlock();
    InterpolatedWorldTime=NonInterpolatedWorldTime+interpolation.Current.WFInterpolate/WorldFramesPerSecond;
    SetDefaultRenderStates();
    CEngineComponentParams params;
    params.FrameInterpolation=interpolation.Current.WFInterpolate;
    params.InterpolationInfo=interpolation;
    params.FrameID=CurrentRenderFrame;
    params.SceneFilterFlags=SceneFilterFlags;
    params.OffWorldTickProcessFlag=OffWorldTickProcessInfo.ProcessThisFrame;
    for(CEngineComponent** i=EngineComponents.begin();i!=EngineComponents.end();++i)
        if((*i)->PeekSceneFilterFlags()&SceneFilterFlags)(*i)->FrameStart(params);
    for(CEngineLayerRenderer** layer=RenderLayers.begin();layer!=RenderLayers.end();++layer)
        (*layer)->Render(params);
    for(CEngineComponent** j=EngineComponents.begin();j!=EngineComponents.end();++j)
        if((*j)->PeekSceneFilterFlags()&SceneFilterFlags)(*j)->FrameEnd(params);
    FrameMemoryPool.FreeAll();
    ++CurrentRenderFrame;
    FableCurrentRenderStateManager_013cb508->RestoreCaptureBlock();
}
