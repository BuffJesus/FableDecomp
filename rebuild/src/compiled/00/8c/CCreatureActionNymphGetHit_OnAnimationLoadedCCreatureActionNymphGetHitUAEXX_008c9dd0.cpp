#include "engine/CCreatureAction_NymphGetHit.h"

extern "C" void FABLE_FASTCALL FableBase_8c9dd0(CCreatureAction_NymphGetHit* self);

struct CCreatureAction_NymphGetHit_Methods : CCreatureAction_NymphGetHit {
    void OnAnimationLoaded();
};

void CCreatureAction_NymphGetHit_Methods::OnAnimationLoaded()
{
    FableBase_8c9dd0(this);
    this->m_b = this->m_a / 2;
}
