#include "engine/CTCHeroRangedAttacks.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefPointeeBase { char pad[0x136]; unsigned char b136; };
bool __fastcall IsUsingProjectileTargetingAnalogueZoom(CTCHeroRangedAttacks* self){ return self->PHeroCombatDef_Object->b136 ? true : false; }