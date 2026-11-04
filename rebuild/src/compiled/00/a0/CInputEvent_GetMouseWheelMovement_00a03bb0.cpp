// CInputEvent::GetMouseWheelMovement (retail 0x00A03BB0).
// FableWin.pdb ?GetMouseWheelMovement@CInputEvent@@QBEMXZ.
#include "fable_input_event.h"

float CInputEvent::GetMouseWheelMovement() const
{
    return MouseMovementStorage_[2];  // MouseMovement.Z
}
