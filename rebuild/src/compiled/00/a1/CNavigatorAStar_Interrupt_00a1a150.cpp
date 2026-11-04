// CNavigatorAStar::Interrupt (retail 0x00A1A150, vtable 0x0129CCCC slot 18). Ego_r.pdb 0xB85050 and
// FableWin.pdb 031b0b50: CNavigatorBase::Interrupt(); LastNavigableNodeVisited = NULL;
// NavigationManager->GetNavigatorQueueManager().RemoveNavigatorFromQueue(*this).
#include "fable_navigator.h"

void CNavigatorAStar::Interrupt()
{
    CNavigatorBase::Interrupt();
    LastNavigableNodeVisited = 0;
    NavigationManager->GetNavigatorQueueManager().RemoveNavigatorFromQueue(*this);
}
