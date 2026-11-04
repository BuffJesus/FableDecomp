// CMainGameComponent::IsControllerDisconnected @ 0x00416296 (31 bytes).
// FableWin ?IsControllerDisconnected@CMainGameComponent@@QBE_NXZ. True when
// the main player's joystick device is no longer active.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_input_event.h"
#include "fable_system_manager.h"

bool CMainGameComponent::IsControllerDisconnected() const
{
    if (g_FableInputManager_013B8388->IsJoystickDeviceNumberActive(
        CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber()))
        return false;
    return true;
}
