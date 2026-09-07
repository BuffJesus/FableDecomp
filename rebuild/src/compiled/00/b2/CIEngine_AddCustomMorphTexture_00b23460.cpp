#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    unsigned long M(int a0, int a1);
};
unsigned long CIEngine_Methods::M(int a0, int a1) { return 0; }