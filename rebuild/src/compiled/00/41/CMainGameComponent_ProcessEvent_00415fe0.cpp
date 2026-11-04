// CMainGameComponent::ProcessEvent @ 0x00415FE0 (18 bytes).
// FableWin ?ProcessEvent@CMainGameComponent@@IAEXPBVCGameEvent@@@Z. The only
// system event the component handles itself is GAME_EVENT_QUIT, which calls
// the SetQuit virtual (CGameComponent slot 4).
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_game_event.h"

void CMainGameComponent::ProcessEvent(const CGameEvent* event)
{
    switch (event->GetType())
    {
    case GAME_EVENT_QUIT:
        SetQuit();
        break;
    }
}
