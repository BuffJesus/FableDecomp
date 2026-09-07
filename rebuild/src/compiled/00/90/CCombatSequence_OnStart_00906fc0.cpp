#include "engine/CCombatSequence_ScorpionLungeAttack.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCombatSequence_ScorpionLungeAttack;
extern int __fastcall re_call(CCombatSequence_ScorpionLungeAttack*);
void __fastcall OnStart(CCombatSequence_ScorpionLungeAttack* self)
{
    self->field28 = re_call(self);
}