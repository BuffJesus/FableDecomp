#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner {
    void Apply(int one, int n, int arg2);
};
struct CWorld { unsigned char pad[0x18]; Inner* p18; };

extern void* __stdcall Resolve(int a);
extern int __fastcall CountOf(void* self);

void __fastcall CGameScriptInterface_TransitionToTheme(CGameScriptInterface* self, void* edx, int arg1, int arg2)
{
    void* r = Resolve(arg1);
    int n = CountOf(r);
    if (n > 0) {
        self->World->p18->Apply(1, n, arg2);
    }
}