#include "fable_input_manager.h"
// Drop processed and stored events, then every device's held events.
void CInputManager::ClearEvents()
{
    ProcessedEvents.clear();
    EmptyEventStore();
    const long count = PJoysticks.size();
    for (long index = 0; index < count; ++index)
    {
        if (PJoysticks[index].Get())
        {
            PJoysticks[index]->Clear();
            PJoysticks[index]->ClearAllMaintainedEvents();
        }
    }
    if (PMouse.get() != 0)
        PMouse->ClearAllMaintainedEvents();
    if (PKeyboard.get() != 0)
        PKeyboard->ClearAllMaintainedEvents();
}
