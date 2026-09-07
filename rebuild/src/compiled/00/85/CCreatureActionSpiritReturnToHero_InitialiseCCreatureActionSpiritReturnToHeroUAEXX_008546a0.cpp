// Byte-exact reconstruction of CCreatureAction_SpiritReturnToHero::Initialise @ 0x008546a0
#include "engine/CCreatureAction_SpiritReturnToHero.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureActionBase { };

// Base class Initialise (call 0x694d10), this-call void
void __fastcall CCreatureActionBase_Initialise(CCreatureAction_SpiritReturnToHero* self);

// Global integer loaded via fild [0x1375550]
extern int g_spiritValue;

void __fastcall Initialise(CCreatureAction_SpiritReturnToHero* self)
{
    CCreatureActionBase_Initialise(self);
    self->field_14 = 0;
    self->field_10 = (long)(float)g_spiritValue;
}