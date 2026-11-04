#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
void CEngine::ChangeCameraAngle()
{
    NEngineInternal::GPPrimitiveOcclusionManager->StartNewScene();
    CameraAngleChanged = true;
}
