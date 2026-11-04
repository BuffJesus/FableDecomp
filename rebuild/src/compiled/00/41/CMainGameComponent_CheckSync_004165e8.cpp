// CMainGameComponent::CheckSync @ 0x004165E8 (134 bytes).
// FableWin ?CheckSync@CMainGameComponent@@IAEXPBVCGameEventPackage@@@Z.
// Walks a received event package and unpacks every frame-update event's
// (checksum, seed, frame) triple for comparison against this machine's world
// checksum and seed. The mismatch report itself (FableWin: when the frame is
// the current server frame and either value differs, a
// CDebugManager::DoErrorMessage) is debug-only and compiled out of retail,
// which is why the unpacked values are never read back.
// MainGameComponent.cpp is size-optimised in retail; the data-package local
// makes this body carry a /GS cookie.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_game_event.h"

void CMainGameComponent::CheckSync(const CGameEventPackage* package)
{
    unsigned long syncChecksum = 0;
    unsigned long syncSeed = 0;
    long syncFrame = 0;
    CGameEventDataPackage data;
    unsigned long worldChecksum = PeekWorldChecksum();
    unsigned long worldSeed = PeekWorldSeed();

    for (unsigned long i = 0; i < package->GetNoEvents(); ++i)
    {
        const CGameEvent* event = package->GetEvent(i);
        if (event->GetType() == GAME_EVENT_UPDATE_FRAME)
        {
            event->GetDataPackage(&data);
            syncChecksum = data.GetULONG();
            syncSeed = data.GetULONG();
            syncFrame = data.GetULONG();
        }
    }
}