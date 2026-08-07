struct PH { char pad[4]; int f4; char pad2[0x10-0x8]; int f10; int f14; int h(int s); };
int __fastcall fn_00c4c2d0(PH* self){ return self->h(self->f10 + self->f14 + self->f4); }