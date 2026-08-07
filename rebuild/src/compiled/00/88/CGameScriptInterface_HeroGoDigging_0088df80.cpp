#include "rebuild_abi.h"
// CGameScriptInterface::HeroGoDigging @ 0x0088df80
// mov ecx,[ecx+0x14]; call h1; push 0; push 0x1e; mov ecx,eax; call h2; ret
// Resolve the hero subsystem (this->f14), then issue a fixed action (0x1e, 0).
struct CHeroSub;
struct CDigTarget { void GoDigging(int a, int b); };
extern "C" CDigTarget *FABLE_FASTCALL FableHeroResolve_88df80(CHeroSub *s);
struct CGameScriptInterface { char pad00[0x14]; CHeroSub *m_hero; void HeroGoDigging(); };
void CGameScriptInterface::HeroGoDigging()
{
    CDigTarget *t = FableHeroResolve_88df80(m_hero);
    t->GoDigging(0x1e, 0);
}