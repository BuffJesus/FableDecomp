#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CGameScriptInterface_Methods : CGameScriptInterface {
    void helperA();
    void helperB(int f);
    void EndCutFade();
};

void CGameScriptInterface_Methods::EndCutFade() {
    this->helperA();
    this->helperB(0x3f000000);
}