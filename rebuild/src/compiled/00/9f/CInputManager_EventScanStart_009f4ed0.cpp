#include "fable_input_manager.h"
bool CInputManager::EventScanStart(CInputEventScanInfo& info, CInputEvent& event) const
{
    info.Started = true;
    ++NoEventScansRunning;
    if (NoStoredEvents > 0)
    {
        info.Index = 0;
        event = *EventStore.begin();
        return true;
    }
    return false;
}
