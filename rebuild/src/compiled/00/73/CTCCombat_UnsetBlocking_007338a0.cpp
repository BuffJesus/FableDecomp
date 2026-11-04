#include "fable_world.h"
void CTCCombat::UnsetBlocking()
{
    CTCCreatureModeManager* modeManager;
    GetParentThing().GetTC(&modeManager);
    modeManager->RemoveMode(12);
    Blocking = false;
    StartingToBlock = false;
}
