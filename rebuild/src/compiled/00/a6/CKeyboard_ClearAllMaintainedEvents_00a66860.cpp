#include "fable_input_devices.h"
void CKeyboard::ClearAllMaintainedEvents()
{
    MaintainedEventsList.clear();
    EmptyEventStore();
}
