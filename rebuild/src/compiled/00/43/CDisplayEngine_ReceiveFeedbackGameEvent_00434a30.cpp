#include "fable_game_event_dispatch.h"

void CDisplayEngine::ReceiveFeedbackGameEvent(const CGameEvent* event) {
    const CGameEventDispatch& dispatch=*Component->PeekEventDispatchTableEntry(event->GetType());
    if (dispatch.FeedbackFunction) (this->*dispatch.FeedbackFunction)(event);
}
