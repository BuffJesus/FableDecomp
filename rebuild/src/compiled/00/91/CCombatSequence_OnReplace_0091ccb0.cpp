#include "engine/CAIStateGroup_MinionReactToNoise.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MinionReactToNoise;
extern int __fastcall re_call(CAIStateGroup_MinionReactToNoise*);
void __fastcall OnReplace(CAIStateGroup_MinionReactToNoise* self)
{
    self->FrameOfLastInteraction = re_call(self);
}