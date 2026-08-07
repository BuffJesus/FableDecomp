#include "rebuild_abi.h"
// CCombatActionBase::GetAnimationTransitionInTime @ 0x008571a0
struct Res { char pad[8]; int m_v; };
struct Sub74 { void Get(Res **out); };
struct CCombatActionBase { char pad[0x74]; Sub74 *m_sub; int GetAnimationTransitionInTime(); };
int CCombatActionBase::GetAnimationTransitionInTime(){
    Res *r;
    m_sub->Get(&r);
    return r->m_v;
}