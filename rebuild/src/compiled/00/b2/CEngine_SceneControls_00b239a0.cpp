#include "fable_engine.h"
void CEngine::StartNewScene(unsigned long flags,float timeout)
{
    NewSceneFlag=true;
    NewSceneLoadFlags=flags;
    NewSceneTimeOut=timeout;
}
void CEngine::CutscenePreload(bool fast,bool)
{
    if(!NewSceneFlag) {
        CutscenePreloadFlag=true;
        if(fast) {
            NewSceneLoadFlags=2;
            NewSceneTimeOut=CutsceneFastPreloadTimeout;
        } else {
            NewSceneLoadFlags=0;
            NewSceneTimeOut=CutscenePreloadTimeout;
        }
    }
}
bool CEngine::IsPreloaded() const
{
    if(NewSceneFlag || CutscenePreloadFlag)return false;
    return true;
}
void CEngine::Enable3DSceneRendering(bool enable)
{
    SceneFilterFlags&=~1UL;
    if(enable)SceneFilterFlags|=1;
}
bool CEngine::Is3DSceneRenderingEnabled() { return (SceneFilterFlags&1)!=0; }
void CEngine::Enable2DSceneRendering(bool enable)
{
    SceneFilterFlags&=~2UL;
    if(enable)SceneFilterFlags|=2;
}
bool CEngine::Is2DSceneRenderingEnabled() { return (SceneFilterFlags&2)!=0; }
