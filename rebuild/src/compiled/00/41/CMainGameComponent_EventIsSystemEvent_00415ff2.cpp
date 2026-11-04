// CMainGameComponent::EventIsSystemEvent @ 0x00415FF2 (19 bytes).
// FableWin ?EventIsSystemEvent@CMainGameComponent@@IAE_NPBVCGameEvent@@@Z.
// GAME_EVENT_QUIT is the one event type the component treats as a system
// event (ProcessEvent handles it; AddGameEvent forwards it to the client).
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_game_event.h"

bool CMainGameComponent::EventIsSystemEvent(const CGameEvent* event)
{
    switch (event->GetType())
    {
    case GAME_EVENT_QUIT:
        return true;
    }
    return false;
}
