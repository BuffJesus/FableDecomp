#include "engine/CTCTargeted.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefPointeeBase { char pad[0x28]; unsigned int defFlags; };
void __fastcall ReturnToDefaultTargetingFlags(CTCTargeted* self)
{
  CDefPointeeBase* s = self->PTargetingDef_Object;
  if (s)
    self->TargetingFlags = s->defFlags;
  else
    self->TargetingFlags = 0;
}