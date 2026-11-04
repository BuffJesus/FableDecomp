#include "engine/CEngineCamera.h"

void CEngineCamera::SetGameCamera(const CCamera* camera)
{
    GameCamera = const_cast<CCamera*>(camera);
}

void CEngineCamera::SetGameWindow(const C2DBoxF* window)
{
    GameWindow = const_cast<C2DBoxF*>(window);
}
