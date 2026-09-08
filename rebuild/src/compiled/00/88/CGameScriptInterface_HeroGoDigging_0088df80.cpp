#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CHeroSub;
struct CDigTarget {
    void GoDigging(int action, int flags);
};

extern "C" CDigTarget* FABLE_FASTCALL FableHeroResolve_88df80(CHeroSub* playerManager);

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void HeroGoDigging();
};

void CGameScriptInterface_Methods::HeroGoDigging()
{
    CDigTarget* target = FableHeroResolve_88df80((CHeroSub*)PlayerManager);
    target->GoDigging(0x1e, 0);
}
