// CGameJoystickManager::GetMainPlayerJoystickDeviceNumber (retail 0x0049BB40).
// FableWin.pdb ?GetMainPlayerJoystickDeviceNumber@CGameJoystickManager@@QBEJXZ.
#include "fable_input_event.h"

long CGameJoystickManager::GetMainPlayerJoystickDeviceNumber() const
{
    return MainPlayerJoystickDeviceNumber;
}
