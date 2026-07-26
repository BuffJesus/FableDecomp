struct Tgt { void method(int a, int b); };
struct B { char pad[0x18]; char* p18; };
struct Self { char pad[4]; B* p4; };

void __fastcall CGameScriptInterface_RadialBlurFadeOut(Self* self, int edx, int a, int b)
{
    char* mid = self->p4->p18;
    Tgt* t = *(Tgt**)(mid + 0x1964);
    t->method(a, b);
}