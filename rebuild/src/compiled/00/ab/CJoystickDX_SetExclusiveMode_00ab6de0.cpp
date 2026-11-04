// CJoystickDX::SetExclusiveMode (retail 0x00AB6DE0): drop the current device and look for a game controller again
// (EnumAndInit takes the first attached one); the joystick is available when that found a device.
#include "fable_dinput8.h"
#include "fable_input_manager.h"
#include "fable_joystick_dx.h"

bool CJoystickDX::SetExclusiveMode(bool)
{
    if (DJoystick)
    {
        DJoystick->Unacquire();
        DJoystick->Release();
        DJoystick = 0;
    }
    ExclusiveMode = true;
    InputManagerDX->GetDInput()->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumAndInit, this, DIEDFL_ATTACHEDONLY);
    return DJoystick ? true : false;
}
