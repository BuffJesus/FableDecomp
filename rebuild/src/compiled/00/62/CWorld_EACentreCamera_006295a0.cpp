// CWorld::EACentreCamera @ 006295A0; FableWin interface names and retail slot 9.
#include "fable_world.h"
#include "fable_game_camera_manager.h"
void CWorld::EACentreCamera(const CGameEvent* event)
{
    CGameCameraManager* manager;
    PCurrentGameCamera->GetCameraManager(&manager);
    manager->RecentreCamera();
}
