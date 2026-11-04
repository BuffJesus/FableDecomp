// CInputEvent::GetType (retail 0x00A03B40).
// FableWin.pdb ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ.
#include "fable_input_event.h"

EInputEventType CInputEvent::GetType() const
{
    return Type;
}
