// CInputEvent::GetKey (retail 0x00A03B70).
// FableWin.pdb ?GetKey@CInputEvent@@QBE?AW4EInputKey@@XZ.
#include "fable_input_event.h"

EInputKey CInputEvent::GetKey() const
{
    return Key;
}
