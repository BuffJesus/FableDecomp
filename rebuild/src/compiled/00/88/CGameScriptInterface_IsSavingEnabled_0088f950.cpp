#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { char pad[0xd7]; char flag; };

char __fastcall CGameScriptInterface_IsSavingEnabled(const CGameScriptInterface* self)
{
    return self->World->flag;
}