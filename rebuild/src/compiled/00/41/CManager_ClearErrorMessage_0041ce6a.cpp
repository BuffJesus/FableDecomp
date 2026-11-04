#pragma optimize("s", on)
// NUISystem::CManager::ClearErrorMessage @ 0x0041CE6A (51 bytes).
// FableWin ?ClearErrorMessage@CManager@NUISystem@@QAEXXZ. Drops the error
// flag (+0x9C), sends the error-message component back to state 0, then
// deletes it and forgets it (+0xA0).
#include "fable_nui_manager.h"
#include "fable_nui_component.h"

void NUISystem::CManager::ClearErrorMessage()
{
    m_bHasErrorMessage = false;
    m_pErrorMessage->ChangeState(0);
    delete m_pErrorMessage;
    m_pErrorMessage = 0;
}
