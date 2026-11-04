// CInputEvent::GetJoystickButton (retail 0x00A03B90).
// FableWin.pdb ?GetJoystickButton@CInputEvent@@QBEJXZ.
#include "fable_input_event.h"

long CInputEvent::GetJoystickButton() const
{
    return JoystickButton;
}
