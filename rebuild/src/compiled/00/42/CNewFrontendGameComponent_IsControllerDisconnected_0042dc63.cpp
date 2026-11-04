// CNewFrontendGameComponent::IsControllerDisconnected @ 0x0042DC63 (49 bytes).
// FableWin ?IsControllerDisconnected@CNewFrontendGameComponent@@AAE_NXZ.
// Only a main-player joystick the player explicitly picked can be
// "disconnected": ask the global input manager whether that device is live.
#include "fable_game.h"
#include "fable_input_event.h"
#include "fable_system_manager.h"

bool CNewFrontendGameComponent::IsControllerDisconnected()
{
    if (CGameJoystickManager::Get().HasExplicitMainPlayerJoystickDeviceNumber())
        return !g_FableInputManager_013B8388->IsJoystickDeviceNumberActive(
            CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber());
    return false;
}
