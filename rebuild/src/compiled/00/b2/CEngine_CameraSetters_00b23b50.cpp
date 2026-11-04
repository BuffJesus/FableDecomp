#include "fable_engine.h"
#include "engine/CEngineCamera.h"
#include "fable_engine_internal.h"

void CEngine::SetCamera(const CCamera* camera)
{
    NEngineInternal::GPCamera->SetGameCamera(camera);
    NEngineInternal::GPCamera->Process(true);
    CurrentCamera = const_cast<CCamera*>(camera);
}

void CEngine::SetWindow(const C2DBoxF* window)
{
    NEngineInternal::GPCamera->SetGameWindow(window);
}
