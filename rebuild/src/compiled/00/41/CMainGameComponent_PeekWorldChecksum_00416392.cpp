// CMainGameComponent::PeekWorldChecksum @ 0x00416392 (37 bytes).
// FableWin ?PeekWorldChecksum@CMainGameComponent@@QBEKXZ. A local game that
// is neither replaying nor recording input events has nothing to keep in
// sync, so the checksum is the constant 1; otherwise it is the world's own.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include "fable_world.h"

unsigned long CMainGameComponent::PeekWorldChecksum() const
{
    if (Client.IsLocal()
        && !g_FableInputManager_013B8388->IsLoadingEvents()
        && !g_FableInputManager_013B8388->IsSavingEvents())
        return 1;
    return PWorld->GetChecksum();
}
