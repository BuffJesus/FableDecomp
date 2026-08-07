struct S { char pad[0x188]; int f188; char pad2[0x88]; int f214; };
void __fastcall Close(S* self){ self->f214 = 0; self->f188 = 0; }