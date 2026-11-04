// CMainGameComponent::AddGameEvent @ 0x0041649C (68 bytes).
// FableWin ?AddGameEvent@CMainGameComponent@@QAEXABVCGameEvent@@@Z. World and
// system events go to the network client; every event is then applied
// locally to the world and fed back to the display engine.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_world.h"

void CMainGameComponent::AddGameEvent(const CGameEvent& event)
{
    if (PWorld->EventIsWorldEvent(&event) || EventIsSystemEvent(&event))
        Client.SendGameEvent(&event);
    PWorld->ReceiveLocalEvent(&event);
    PDisplayEngine->ReceiveFeedbackGameEvent(&event);
}
