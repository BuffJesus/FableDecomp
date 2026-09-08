#include "engine/CTCDoor.h"

struct CDoorDefinitionView {
    unsigned char padding[0x38];
    unsigned char vandalismCrimeEnabled;
};

struct CTCDoor_Methods : CTCDoor {
    bool PostVandalismCrimeIfDestroyed();
};

bool CTCDoor_Methods::PostVandalismCrimeIfDestroyed()
{
    CDoorDefinitionView* definition = (CDoorDefinitionView*)this->PDef_Object;
    if (definition && definition->vandalismCrimeEnabled) {
        return true;
    }
    return false;
}
