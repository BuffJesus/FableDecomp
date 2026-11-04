// CMainGameComponent::UpdateFromEventPackageSet @ 0x0041726D (187 bytes).
// FableWin ?UpdateFromEventPackageSet@CMainGameComponent@@IAEXPBVCGameEventPackageSet@@@Z.
// Applies every package newer than the last input frame: process its events,
// update the world (rendering allowed only when input has caught up with the
// server frame), confirm it to the display engine, advance the input and
// server frames, then report the new frame and checksum to the network client
// and let the display engine catch up with the world.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_game_event.h"
#include "fable_world.h"

// The engine's generic max (Ego_r.pdb ??$Maximum@N@@YAABNABN0@Z is the double
// instance); inline here.
template<class T> inline const T& Maximum(const T& left, const T& right)
{
    return left < right ? right : left;
}

void CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet* packageSet)
{
    for (unsigned long i = 0; i < packageSet->GetNoPackages(); ++i)
    {
        const CGameEventPackage* package = packageSet->GetPackage(i);
        if (package->GetTimeStamp() > InputServerFrame)
        {
            ProcessEventPackage(package);

            bool allowRender = false;
            if (InputServerFrame == CurrentServerFrame)
            {
                allowRender = true;
                AllowRender = true;
            }
            PWorld->Update(*package, allowRender);
            PDisplayEngine->ConfirmFeedbackGameEventPackage(package);

            InputServerFrame = package->GetTimeStamp();
            CurrentServerFrame = Maximum(package->GetTimeStamp(), CurrentServerFrame);
            Client.Update(GetCurrentServerFrame(), PeekWorldChecksum(), PeekWorldSeed());
            PDisplayEngine->WorldUpdate();
        }
    }
}