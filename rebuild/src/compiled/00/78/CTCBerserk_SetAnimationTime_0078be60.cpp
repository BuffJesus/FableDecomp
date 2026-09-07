#include "engine/CTCBerserk.h"  // retyped onto the PDB layout; byte parity re-verified
extern int g_someint;
void __fastcall SetAnimationTime(CTCBerserk* self, float a){ self->ReleasedAnimationTimer = (float)g_someint * a; }