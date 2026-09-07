#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct C { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
          virtual void v4(); virtual void v5(); virtual void SetX(bool); };
struct B { char pad[0x2c]; C* f2c; };
struct CMainGameComponent { char pad[0x28]; B* f28; };
struct CGameScriptInterface_Methods : CGameScriptInterface {
    void SetEnvironmentalEffectsAlwaysUpdate(bool b);
};
void CGameScriptInterface_Methods::SetEnvironmentalEffectsAlwaysUpdate(bool b){ Component->f28->f2c->SetX(b); }