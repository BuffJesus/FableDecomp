#include "engine/CEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngine_Methods : CEngine {
    bool M(int a0, int a1, int a2);
};
bool CEngine_Methods::M(int a0, int a1, int a2) { return false; }