// CInputEvent::GetMousePos (retail 0x00A04380).
// Ego_r ?GetMousePos@CInputEvent@@QBE?AVC2DVector@@XZ, first of the by-value
// getters before GetJoystickPosition (0x00A043A0); retail callers in
// CInputManager::UpdateMaintainedEvents use it on the same event after GetType.
#include "fable_input_event.h"

C2DVector CInputEvent::GetMousePos() const
{
    return MousePos;
}
