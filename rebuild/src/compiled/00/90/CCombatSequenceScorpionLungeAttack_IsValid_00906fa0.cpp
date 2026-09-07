// Byte-exact reconstruction of CCombatSequence_ScorpionLungeAttack::IsValid @ 0x00906fa0

#include "engine/CCombatSequence_ScorpionLungeAttack.h"  // retyped onto the PDB layout; byte parity re-verified
extern int g_globalMultiplier;                 // ds:0x1375550
extern "C" int __cdecl GetCurrentValue();      // 0x49d870

bool __fastcall IsValid(CCombatSequence_ScorpionLungeAttack* self)
{
    int mult = g_globalMultiplier;
    if (GetCurrentValue() - self->field28 > mult * 10)
        return true;
    return false;
}