// CMainGameComponent::ProcessEventPackage @ 0x00416670.
#pragma optimize("s", on)
#include "fable_game.h"
#include "fable_game_event.h"
void CMainGameComponent::ProcessEventPackage(const CGameEventPackage* package)
{
    for (unsigned long index = 0; index < package->GetNoEvents(); ++index)
        ProcessEvent(package->GetEvent(index));
}
