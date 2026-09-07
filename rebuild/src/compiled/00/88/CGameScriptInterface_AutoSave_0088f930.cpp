#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld {
    void Save(int a);
};


void __fastcall CGameScriptInterface_AutoSave(CGameScriptInterface* self)
{
    self->World->Save(0);
}