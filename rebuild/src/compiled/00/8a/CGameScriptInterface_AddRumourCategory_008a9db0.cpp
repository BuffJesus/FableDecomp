#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    unsigned int value;
    CCharString(const CCharString& other);
};

extern void __stdcall CGameScriptInterface_FindOrCreateRumourCategory(CCharString category);

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void AddRumourCategory(const CCharString& category) const;
};

void CGameScriptInterface_Methods::AddRumourCategory(const CCharString& category) const
{
    CGameScriptInterface_FindOrCreateRumourCategory(category);
}