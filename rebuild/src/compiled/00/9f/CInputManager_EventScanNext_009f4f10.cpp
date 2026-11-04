#include "fable_input_manager.h"
// FableWin.pdb ?EventScanNext@CInputManager@@QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z.
// Next stored event after info.Index whose type is not IE_NULL.
bool CInputManager::EventScanNext(CInputEventScanInfo& info, CInputEvent& event) const
{
    for (long index = info.Index + 1; index < NoStoredEvents; ++index)
    {
        const CInputEvent& stored = EventStore[index];
        if (stored.GetType() != IE_NULL)
        {
            info.Index = index;
            event = stored;
            return true;
        }
    }
    return false;
}
