#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Tgt { void method(int a, int b); };
struct CWorld { char pad[0x18]; char* p18; };

void __fastcall CGameScriptInterface_RadialBlurFadeOut(CGameScriptInterface* self, int edx, int a, int b)
{
    char* mid = self->World->p18;
    Tgt* t = *(Tgt**)(mid + 0x1964);
    t->method(a, b);
}