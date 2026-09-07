// C3DAnimationTransitionState::FindDominantAnimation @ 0x00710540
#include "engine/C3DAnimationTransitionState.h"  // retyped onto the PDB layout; byte parity re-verified
struct C3DAnimationState;

struct CIVCountedPointeeBase {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual const C3DAnimationState& FindDominant() = 0; // slot 3 (offset 0xc)
};


extern const float g_threshold; // [0x122f59c]

const C3DAnimationState& __fastcall FindDominantAnimation(C3DAnimationTransitionState* self)
{
    if (self->Progress <= g_threshold)
        return self->StartState_Object->FindDominant();
    return self->EndState_Object->FindDominant();
}