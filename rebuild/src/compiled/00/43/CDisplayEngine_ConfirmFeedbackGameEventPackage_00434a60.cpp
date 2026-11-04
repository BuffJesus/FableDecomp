#include "fable_game_event_dispatch.h"

void CDisplayEngine::ConfirmFeedbackGameEventPackage(const CGameEventPackage* package) {
    for (unsigned long i=0; i<package->GetNoEvents(); ++i) {
        const CGameEvent* event=package->GetEvent(i);
        const CGameEventDispatch& dispatch=*Component->PeekEventDispatchTableEntry(event->GetType());
        if (dispatch.ConfirmFunction) (this->*dispatch.ConfirmFunction)(event);
    }
}
