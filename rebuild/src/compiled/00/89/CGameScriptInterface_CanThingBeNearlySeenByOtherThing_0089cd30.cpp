#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CThingChecker {
    bool CanThingBeSeen(void* a, void* b, int mode);
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    bool CanThingBe_NearlySeen_ByOtherThing(void* thingA, void* thingB);
};

bool CGameScriptInterface_Methods::CanThingBe_NearlySeen_ByOtherThing(void* thingA, void* thingB)
{
    return ((CThingChecker*)this)->CanThingBeSeen(thingA, thingB, 2);
}