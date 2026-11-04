#include "fable_world.h"
#include "fable_thing_manager.h"

unsigned long CWorld::GetChecksum() const
{
    unsigned long checksum = PThingManager->GetNonLocalThingChecksum();
    long frame = Frame;
    return checksum + frame;
}
