#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct GObj { char pad[0x1010]; unsigned char b0:1; unsigned char b1:1; };
extern GObj* GetDecalObj(void);

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void CGameScriptInterface_EnableDecals(char enable);
};

void CGameScriptInterface_Methods::CGameScriptInterface_EnableDecals(char enable)
{
    GObj* o = GetDecalObj();
    o->b1 = enable;
}