#include "fable_input_manager.h"
// FableWin.pdb ?EventScanEnd@CInputManager@@QBEXAAVCInputEventScanInfo@@@Z.
void CInputManager::EventScanEnd(CInputEventScanInfo& info) const
{
    info.Started = false;
    --NoEventScansRunning;
}
