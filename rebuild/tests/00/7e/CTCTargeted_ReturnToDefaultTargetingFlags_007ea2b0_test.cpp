#include "engine/CTCTargeted.h"
#include <stdio.h>
struct CDefPointeeBase { char pad[0x28]; unsigned int defFlags; };
void __fastcall ReturnToDefaultTargetingFlags(CTCTargeted* self)
{
  CDefPointeeBase* s = self->PTargetingDef_Object;
  if (s)
    self->TargetingFlags = s->defFlags;
  else
    self->TargetingFlags = 0;
}
int main(){
  CDefPointeeBase s; s.defFlags = 0x1234;
  CTCTargeted a; a.TargetingFlags = 0x999; a.PTargetingDef_Object = &s;
  ReturnToDefaultTargetingFlags(&a);
  if (a.TargetingFlags != 0x1234) { printf("FAIL1\n"); return 1; }
  CTCTargeted b; b.TargetingFlags = 0x999; b.PTargetingDef_Object = 0;
  ReturnToDefaultTargetingFlags(&b);
  if (b.TargetingFlags != 0) { printf("FAIL2\n"); return 1; }
  printf("OK_007ea2b0\n");
  return 0;
}