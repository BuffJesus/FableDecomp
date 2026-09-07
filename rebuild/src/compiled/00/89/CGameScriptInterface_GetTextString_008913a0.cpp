// CGameScriptInterface_Methods::GetTextString @ 0x008913a0
// Returns CWideString by value; forwards to a member helper obtained from a global.

#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString { char* p; };
struct CWideString { unsigned short* p; };

// Subobject at global[0x14] whose member GetText builds a CWideString by value.
struct Sub {
    CWideString GetText(CCharString const& str);
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    CWideString GetTextString(CCharString const& str);
};

// global object; +0x14 = Sub*
extern char* g_globalObj;

CWideString CGameScriptInterface_Methods::GetTextString(CCharString const& str)
{
    Sub* sub = *(Sub**)(g_globalObj + 0x14);
    return sub->GetText(str);
}