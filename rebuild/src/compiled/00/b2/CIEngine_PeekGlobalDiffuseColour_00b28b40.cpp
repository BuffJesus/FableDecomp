// CIEngine_Methods::PeekGlobalDiffuseColour @ 00b28b40

#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CRGBFloatColour {
    float r, g, b, a;
    CRGBFloatColour(float rr, float gg, float bb, float aa) : r(rr), g(gg), b(bb), a(aa) {}
    CRGBFloatColour() {}
};

struct CIEngine_Methods : CIEngine {
    CRGBFloatColour PeekGlobalDiffuseColour();
};

CRGBFloatColour CIEngine_Methods::PeekGlobalDiffuseColour()
{
    return CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f);
}