#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    bool M(int x0, int x1);
};
bool CIEngine_Methods::M(int x0, int x1) { return true; }