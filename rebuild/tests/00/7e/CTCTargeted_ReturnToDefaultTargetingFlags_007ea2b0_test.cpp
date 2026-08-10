#include <stdio.h>
struct Src { char pad[0x28]; unsigned int defFlags; };
struct CTCTargeted {
  char pad0[0xc];
  unsigned int curFlags;
  char pad1[0x04];
  Src* src;
};
void __fastcall ReturnToDefaultTargetingFlags(CTCTargeted* self)
{
  Src* s = self->src;
  if (s)
    self->curFlags = s->defFlags;
  else
    self->curFlags = 0;
}
int main(){
  Src s; s.defFlags = 0x1234;
  CTCTargeted a; a.curFlags = 0x999; a.src = &s;
  ReturnToDefaultTargetingFlags(&a);
  if (a.curFlags != 0x1234) { printf("FAIL1\n"); return 1; }
  CTCTargeted b; b.curFlags = 0x999; b.src = 0;
  ReturnToDefaultTargetingFlags(&b);
  if (b.curFlags != 0) { printf("FAIL2\n"); return 1; }
  printf("OK_007ea2b0\n");
  return 0;
}