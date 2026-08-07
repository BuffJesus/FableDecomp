struct S { char pad[8]; int f8; int fC; };
void __fastcall SetDefaultValue(S* self){ self->fC = -999; self->f8 = -777; }