#include "fable_input_devices.h"
void CMouse::ClearAllMaintainedEvents()
{
    MaintainedEventsList.clear();
    EmptyEventStore();
}
