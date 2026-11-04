#include "fable_game_camera_manager.h"

bool CGameCameraManager::GetCameraManager(CGameCameraManager** manager)
{
    *manager = this;
    return true;
}
