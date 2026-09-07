#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { char pad[0xda]; unsigned char b_da; };
struct CGameScriptInterface_Methods : CGameScriptInterface {
    void Set(unsigned char v);
};
void CGameScriptInterface_Methods::Set(unsigned char v){ this->World->b_da = v; }