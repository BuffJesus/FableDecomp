#include "fable_game_camera_manager.h"

bool CGameCameraManager::GetCameraManager(const CGameCameraManager** manager) const
{
    *manager = this;
    return true;
}
