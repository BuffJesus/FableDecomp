extern int g_someint;
struct S { char pad[0x1c]; float f1c; };
void __fastcall SetAnimationTime(S* self, float a){ self->f1c = (float)g_someint * a; }