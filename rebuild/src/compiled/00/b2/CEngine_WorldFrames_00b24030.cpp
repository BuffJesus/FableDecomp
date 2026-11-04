#include "fable_engine.h"
#include "fable_engine_component.h"
void CEngine::WorldFrameUpdate(bool firstFrame)
{
    if(IsInitialised && CurrentRenderFrame) {
        for(CEngineComponent** component=EngineComponents.begin();component!=EngineComponents.end();++component) {
            if((*component)->PeekSceneFilterFlags() & SceneFilterFlags)
                (*component)->WorldFrameUpdate(firstFrame);
        }
    }
    ++CurrentWorldFrame;
    NonInterpolatedWorldTime+=1.0f/WorldFramesPerSecond;
}
void CEngine::PausedWorldFrameUpdate()
{
    if(IsInitialised && CurrentRenderFrame) {
        for(CEngineComponent** component=EngineComponents.begin();component!=EngineComponents.end();++component) {
            if((*component)->PeekSceneFilterFlags() & SceneFilterFlags)
                (*component)->PausedWorldFrameUpdate();
        }
    }
    ++CurrentPausedWorldFrame;
    NonInterpolatedPausedWorldTime+=1.0f/WorldFramesPerSecond;
}
