struct S { char pad[8]; int f8; int fC; };
void __fastcall SetDefaultValue(S* self){ self->f8 = 2; self->fC = 0; }