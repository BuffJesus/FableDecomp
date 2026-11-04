// CNetworkClient::GetLocalGameEventPackageSet, retail 0x004AEAA0
// (FableWin ?GetLocalGameEventPackageSet@CNetworkClient@@AAE_NPAVCGameEventPackageSet@@@Z).
// Local games synthesise the server's frame-update event themselves: when the
// component says it is time for the next server frame, pack (Checksum1,
// Checksum2, LocalFrame) into an update-frame event, advance the frame, and
// hand the one-package set to the caller. Built with /GS (retail cookie check).
#include "fable_game.h"

inline bool CNetworkClient::CheckForLocalFrameUpdate() const
{
    return GameComponent->IsTimeForServerUpdate(LocalFrame);
}

bool CNetworkClient::GetLocalGameEventPackageSet(CGameEventPackageSet* packageSet)
{
    CGameEvent event;
    if (CheckForLocalFrameUpdate() == true)
    {
        event.Initialise(GAME_EVENT_UPDATE_FRAME);
        event.SetPlayer(-1);
        event.AddULONG(Checksum1);
        event.AddULONG(Checksum2);
        event.AddULONG(LocalFrame);
        ++LocalFrame;
        LocalEventPackage.AddEvent(&event);
        LocalEventPackage.SetTimeStamp(LocalFrame);
        packageSet->Clear();
        packageSet->AddPackage(&LocalEventPackage);
        LocalEventPackage.Clear();
        return true;
    }
    return false;
}