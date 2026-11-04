// CJoystickDX's focus members (retail 0x00AB6D10, 0x00AB6D60): the joystick takes the DirectInput device while the
// application has the input focus and gives it back when it loses it (unlike the mouse, without checking the result).
#include "fable_dinput8.h"
#include "fable_joystick_dx.h"
#include "fable_gfuninitialise.h"

// 0x00AB6D10
void CJoystickDX::SetHasInputFocus(bool hasFocus)
{
    if (DJoystick)
    {
        if (!hasFocus)
        {
            if (Acquired == true)
            {
                DJoystick->Unacquire();
                Acquired = false;
            }
        }
        else if (!Acquired)
        {
            DJoystick->Acquire();
            Acquired = true;
        }
    }
}

// 0x00AB6D60
void CJoystickDX::UpdateAcquired()
{
    if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->IsApplicationInputFocus())
        SetHasInputFocus(true);
    else
        SetHasInputFocus(false);
}
