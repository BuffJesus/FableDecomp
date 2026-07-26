struct CPlayerGui {
  char pad0[0x0d];
  unsigned char b0d;
  unsigned char b0e;
  unsigned char b0f;
  unsigned char b10;
  unsigned char b11;
  unsigned char b12;
  unsigned char b13;
  unsigned char b14;
  unsigned char b15;
  unsigned char b16;
  unsigned char b17;
  unsigned char b18;
  unsigned char b19;
  char pad1a[0x20-0x1a];
  float f20;
  char pad24[0x32-0x24];
  unsigned char b32;
  char pad33[0x6c-0x33];
  unsigned char b6c;
  char pad6d[0x79-0x6d];
  unsigned char b79;
  char pad7a[0x7c-0x7a];
  float f7c;
};
extern void __fastcall CPlayerGui_Reset(CPlayerGui* self);
void __fastcall CPlayerGui_ForceClose(CPlayerGui* self)
{
  self->b0d = 1;
  self->b0e = 1;
  self->b0f = 0;
  self->b10 = 0;
  self->b11 = 0;
  self->b12 = 0;
  self->b13 = 0;
  self->b14 = 0;
  self->b15 = 0;
  self->b16 = 0;
  self->b17 = 1;
  self->b18 = 1;
  self->b19 = 1;
  self->b32 = 0;
  self->f20 = 1.0f;
  self->b6c = 1;
  self->b79 = 1;
  self->f7c = 0.5f;
  CPlayerGui_Reset(self);
}