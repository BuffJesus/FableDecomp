#include "fable_input_devices.h"
void CJoystick::ClearAllMaintainedEvents()
{
    MaintainedEventsList.clear();
    EmptyEventStore();
}
