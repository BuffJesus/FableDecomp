// CInputEvent::GetDeviceType (retail 0x00A03B50).
// Ego_r ?GetDeviceType@CInputEvent@@QBE?AW4EInputDeviceType@@XZ, between GetType
// (0x00A03B40) and GetDeviceNumber (0x00A03B60) in lib_input_event order.
#include "fable_input_event.h"

EInputDeviceType CInputEvent::GetDeviceType() const
{
    return DeviceType;
}
