// CInputEvent::GetJoystickButtonPressureFraction (retail 0x00A03BA0).
// Ego_r ?GetJoystickButtonPressureFraction@CInputEvent@@QBEMXZ, between
// GetJoystickButton (0x00A03B90) and GetMouseWheelMovement (0x00A03BB0).
#include "fable_input_event.h"

float CInputEvent::GetJoystickButtonPressureFraction() const
{
    return JoystickButtonPressureFraction;
}
