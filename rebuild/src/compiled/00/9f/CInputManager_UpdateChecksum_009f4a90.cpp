#include "fable_input_manager.h"
// FableWin.pdb ?UpdateChecksum@CInputManager@@QAEXJABVCChecksum@@@Z.
void CInputManager::UpdateChecksum(long timeStamp, const CChecksum& checksum)
{
    Checksum = checksum;
    TimeStamp = timeStamp;
}
