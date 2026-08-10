#include <cstdio>
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
int main(){
    CIEngine e;
    CRGBFloatColour c = e.PeekGlobalDiffuseColour();
    if (c.r==1.0f && c.g==1.0f && c.b==1.0f && c.a==1.0f) printf("00b28b40_TEST PASS\n");
    else printf("FAIL %f %f %f %f\n", c.r,c.g,c.b,c.a);
    return 0;
}