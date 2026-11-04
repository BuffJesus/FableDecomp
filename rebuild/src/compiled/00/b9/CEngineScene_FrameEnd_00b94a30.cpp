#include "engine/CEngineScene.h"
void CEngineScene::FrameEnd()
{
    UpdateMaps();
    ClearNonPersistent();
}
