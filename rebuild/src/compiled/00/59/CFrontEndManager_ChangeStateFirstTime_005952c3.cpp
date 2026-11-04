#pragma optimize("s", on)
// NUISystem::CFrontEndManager::ChangeStateFirstTime @ 0x005952C3 (21 bytes).
// FableWin ?ChangeStateFirstTime@CFrontEndManager@NUISystem@@QAEXXZ: puts
// the screen on top of the screen stack (+0x20) into state 5.
// stack<CComponent*>::top() stays out of line in retail (0x0059B039).
#include "fable_nui_manager.h"
#include "fable_nui_component.h"

void NUISystem::CFrontEndManager::ChangeStateFirstTime()
{
    m_stackScreens.top()->ChangeState(5);
}
