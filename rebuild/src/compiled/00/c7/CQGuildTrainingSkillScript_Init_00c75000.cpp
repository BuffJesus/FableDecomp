struct S { char pad[8]; int f8; int fc; char pad2[8]; int f18; int f1c; };
void __fastcall Init(S* self){ self->f8=0; self->fc=0; self->f18=0; self->f1c=0; }