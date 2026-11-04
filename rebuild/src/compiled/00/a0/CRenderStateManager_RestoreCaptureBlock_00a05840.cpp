#include "fable_render_state_manager.h"

void CRenderStateManager::RestoreCaptureBlock()
{
    CStateInfo* entry = StateStack[--StateStackSize].State;
    while (entry->StateType != 4)
    {
        entry->BookmarkMask -= CurrentBookmarkMask;
        entry->DesiredState = StateStack[StateStackSize].Value;
        AddStateToUpdateList(*entry);
        entry = StateStack[--StateStackSize].State;
    }
    CurrentBookmarkMask >>= 1;
}
