// CMainGameComponent::UninitEventPackageLoading @ 0x004160A6 (38 bytes).
// FableWin ?UninitEventPackageLoading@CMainGameComponent@@IAEXXZ. Closes the
// event-package file if it is open and clears LoadingEventPackages.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"

void CMainGameComponent::UninitEventPackageLoading()
{
    if (EventPackageFile.IsOpen())
        EventPackageFile.Close();
    LoadingEventPackages = false;
}
