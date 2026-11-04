#include "fable_input_devices.h"
bool CJoystick::EventScanNext(CInputEventScanInfo* info, CInputEvent* event) const
{
    long index = info->Index + 1;
    if (index < NoStoredEvents)
    {
        info->Index = index;
        *event = EventStore[index];
        return true;
    }
    return false;
}
