#include "rebuild_abi.h"
// C3DAnimationBlendState::AddCombinationBlend @ 0x00662430
struct C3DAnimationBlendState;
struct Sub { void Add(C3DAnimationBlendState *self); };
struct Obj { char pad[0x4c]; Sub *m_sub; };
extern "C" Obj *FABLE_FASTCALL FableGet_662430(C3DAnimationBlendState *self);
struct C3DAnimationBlendState { char dummy; void AddCombinationBlend(); };
void C3DAnimationBlendState::AddCombinationBlend(){
    Sub *s = FableGet_662430(this)->m_sub;
    s->Add(this);
}