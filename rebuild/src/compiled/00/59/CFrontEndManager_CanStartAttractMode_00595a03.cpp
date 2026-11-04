// NUISystem::CFrontEndManager::CanStartAttractMode @ 0x00595A03 (3 bytes).
// FableWin ?CanStartAttractMode@CFrontEndManager@NUISystem@@QAE_NXZ. The PC
// build never starts the attract-mode demo: `xor al,al; ret` (retail folds
// other `return false` bodies onto this address).
#include "fable_nui_manager.h"

bool NUISystem::CFrontEndManager::CanStartAttractMode()
{
    return false;
}
