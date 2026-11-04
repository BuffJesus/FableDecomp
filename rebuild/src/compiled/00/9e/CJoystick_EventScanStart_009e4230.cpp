#include "fable_input_devices.h"
bool CJoystick::EventScanStart(CInputEventScanInfo* info, CInputEvent* event) const
{
    info->Started = true;
    ++NoEventScansRunning;
    if (NoStoredEvents > 0)
    {
        info->Index = 0;
        *event = EventStore[0];
        return true;
    }
    return false;
}
