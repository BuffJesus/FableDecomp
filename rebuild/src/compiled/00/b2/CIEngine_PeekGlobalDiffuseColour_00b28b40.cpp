// CIEngine::PeekGlobalDiffuseColour @ 00b28b40

struct CRGBFloatColour {
    float r, g, b, a;
    CRGBFloatColour(float rr, float gg, float bb, float aa) : r(rr), g(gg), b(bb), a(aa) {}
    CRGBFloatColour() {}
};

struct CIEngine {
    CRGBFloatColour PeekGlobalDiffuseColour();
    char dummy;
};

CRGBFloatColour CIEngine::PeekGlobalDiffuseColour()
{
    return CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f);
}