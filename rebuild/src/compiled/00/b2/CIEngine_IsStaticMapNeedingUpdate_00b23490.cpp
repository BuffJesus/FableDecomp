#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    bool M(int a0, int a1, int a2);
};
bool CIEngine_Methods::M(int a0, int a1, int a2) { return false; }