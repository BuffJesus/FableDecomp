struct S { char pad[4]; int f4; char pad2[0xa0]; unsigned char b_a8; };
void __fastcall Init(S* self){ self->b_a8 = 0; self->f4 = 2; }