struct Src { char pad[0x28]; unsigned int defFlags; };
struct CTCTargeted {
  char pad0[0xc];
  unsigned int curFlags;   // +0x0c
  char pad1[0x04];
  Src* src;                // +0x14
};
void __fastcall ReturnToDefaultTargetingFlags(CTCTargeted* self)
{
  Src* s = self->src;
  if (s)
    self->curFlags = s->defFlags;
  else
    self->curFlags = 0;
}