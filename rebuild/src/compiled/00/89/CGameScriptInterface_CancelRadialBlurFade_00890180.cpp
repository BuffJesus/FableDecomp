#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTarget;
extern void __fastcall Callee(CTarget* self);

struct CInner {
    char pad[0x1964];
    CTarget* fld;
};
struct CWorld {
    char pad[0x18];
    CInner* inner;
};

void __fastcall CGameScriptInterface_CancelRadialBlurFade(CGameScriptInterface* self)
{
    Callee(self->World->inner->fld);
}