struct S { char pad[0x10]; float* f10; char pad2[8]; unsigned char b1c; };
void __fastcall SetAngle(S* self, float a){ float* p = self->f10; self->b1c = 1; *p = a; }