// CMainGameComponent::UninitEventPackageSaving @ 0x00416122 (38 bytes).
// FableWin ?UninitEventPackageSaving@CMainGameComponent@@IAEXXZ. Closes the
// event-package file if it is open and clears SavingEventPackages.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"

void CMainGameComponent::UninitEventPackageSaving()
{
    if (EventPackageFile.IsOpen())
        EventPackageFile.Close();
    SavingEventPackages = false;
}
