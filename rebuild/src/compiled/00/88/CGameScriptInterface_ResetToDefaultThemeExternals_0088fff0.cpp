#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner;
extern void __fastcall Inner_Do(Inner* self, int e, int a, int b, int c);
struct CWorld { char pad[0x18]; Inner* p18; };

void __fastcall CGameScriptInterface_ResetToDefaultThemeExternals(CGameScriptInterface* self, int edx, int arg)
{
    Inner* obj = self->World->p18;
    Inner_Do(obj, edx, 0, 1, arg);
}