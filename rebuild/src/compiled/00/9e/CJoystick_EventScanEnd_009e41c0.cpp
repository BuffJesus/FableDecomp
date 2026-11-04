#include "fable_input_devices.h"
void CJoystick::EventScanEnd(CInputEventScanInfo* info) const
{
    info->Started = false;
    --NoEventScansRunning;
}
