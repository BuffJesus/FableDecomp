#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CThing;
struct CGameScriptInterface_Methods : CGameScriptInterface {
    bool CanThingBe_Seen_ByOtherThing(CThing* a, CThing* b);
    bool CanSeeImpl(CThing* a, CThing* b, int mode);
};

bool CGameScriptInterface_Methods::CanThingBe_Seen_ByOtherThing(CThing* a, CThing* b)
{
    return CanSeeImpl(a, b, 1);
}