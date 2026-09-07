#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    void* p;
    CCharString(const CCharString& o);
};

struct CGlobal { char pad[0xb0]; CCharString name; };
extern CGlobal* g_obj;

struct CGameScriptInterface_Methods : CGameScriptInterface {
    CCharString GetMostRecentValidUsedTargetName();
};

CCharString CGameScriptInterface_Methods::GetMostRecentValidUsedTargetName()
{
    return g_obj->name;
}