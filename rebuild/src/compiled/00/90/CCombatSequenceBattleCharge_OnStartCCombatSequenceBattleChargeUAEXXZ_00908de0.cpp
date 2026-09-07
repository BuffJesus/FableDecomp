#include "engine/CCombatSequence_BattleCharge.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
struct G { int Helper(); };
extern G* g_908de0;
struct CCombatSequence_BattleCharge_Methods : CCombatSequence_BattleCharge {
    void OnStart();
};
void CCombatSequence_BattleCharge_Methods::OnStart(){ this->f28 = g_908de0->Helper(); }