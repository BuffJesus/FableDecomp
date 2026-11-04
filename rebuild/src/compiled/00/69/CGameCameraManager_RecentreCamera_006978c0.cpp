// CGameCameraManager::RecentreCamera @ 006978C0; mode Data +98, Recentre slot 16.
#include "fable_game_camera_manager.h"
#include "fable_camera_mode.h"
void CGameCameraManager::RecentreCamera()
{
    NCameraModes::CBase* mode = CurrentMode.Get();
    if (mode)
        mode->Recentre();
}
