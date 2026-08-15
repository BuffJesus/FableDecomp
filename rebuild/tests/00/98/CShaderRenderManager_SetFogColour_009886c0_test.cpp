#include <stdio.h>
#include <string.h>

struct CRGBFloatColour { float r, g, b, a; };

struct CShaderRenderManager {
    char pad0[0x1bc];
    CRGBFloatColour fog;
    char pad1[0x3d8 - 0x1bc - 16];
    unsigned int flags;
    void SetFogColour(CRGBFloatColour* src);
};

void CShaderRenderManager::SetFogColour(CRGBFloatColour* src)
{
    this->fog = *src;
    this->flags |= 0x20000;
}

int main()
{
    CShaderRenderManager m;
    memset(&m, 0, sizeof(m));
    m.flags = 0x1;
    CRGBFloatColour c;
    c.r = 1.0f; c.g = 2.0f; c.b = 3.0f; c.a = 4.0f;
    m.SetFogColour(&c);
    int ok = (m.fog.r == 1.0f && m.fog.g == 2.0f && m.fog.b == 3.0f &&
              m.fog.a == 4.0f && m.flags == (0x1 | 0x20000));
    if (ok) printf("PARITY_OK\n");
    else printf("FAIL flags=%x\n", m.flags);
    return 0;
}