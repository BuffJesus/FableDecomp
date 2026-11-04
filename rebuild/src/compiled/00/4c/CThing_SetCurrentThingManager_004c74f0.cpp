#include "fable_world.h"
void FABLE_FASTCALL CThing::SetCurrentThingManager(CThingManager& manager)
{
    PCurrentThingManager = &manager;
}

// Retail zero-initialised singleton storage, 0x013B8A1C.
CThingManager* CThing::PCurrentThingManager = 0;
