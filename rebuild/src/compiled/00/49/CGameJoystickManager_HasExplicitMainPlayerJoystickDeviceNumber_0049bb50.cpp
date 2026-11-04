// CGameJoystickManager::HasExplicitMainPlayerJoystickDeviceNumber (retail 0x0049BB50).
// FableWin.pdb ?HasExplicitMainPlayerJoystickDeviceNumber@CGameJoystickManager@@QBE_NXZ.
#include "fable_input_event.h"

bool CGameJoystickManager::HasExplicitMainPlayerJoystickDeviceNumber() const
{
    return MainPlayerJoystickDeviceNumberSet;
}
