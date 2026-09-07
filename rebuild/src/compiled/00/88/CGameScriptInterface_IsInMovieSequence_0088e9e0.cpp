#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CGameScriptInterface_Methods : CGameScriptInterface {
    unsigned char M();
};
unsigned char CGameScriptInterface_Methods::M(){ return this->InMovieSequence; }