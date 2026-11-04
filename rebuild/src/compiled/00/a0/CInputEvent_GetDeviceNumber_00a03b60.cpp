// CInputEvent::GetDeviceNumber (retail 0x00A03B60).
// FableWin.pdb ?GetDeviceNumber@CInputEvent@@QBEJXZ.
#include "fable_input_event.h"

long CInputEvent::GetDeviceNumber() const
{
    return DeviceNumber;
}
