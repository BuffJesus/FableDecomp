#include "engine/CEngineScene.h"
void CEngineScene::ResetStats()
{
    RenderStats.clear();
    UpdateStats=CEngineSceneUpdateStats();
}
