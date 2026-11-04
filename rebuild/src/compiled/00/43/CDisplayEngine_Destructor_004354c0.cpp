#include "fable_display_engine.h"
#include "fable_engine_manager.h"
#include "fable_iengine.h"

void CDisplayEngine::UninitEngine()
{
    Engine3D->Release();
    CEngineManager::Get().RemoveEngine(*Engine3D);
}
CDisplayEngine::~CDisplayEngine()
{
    UninitEngine();
}
