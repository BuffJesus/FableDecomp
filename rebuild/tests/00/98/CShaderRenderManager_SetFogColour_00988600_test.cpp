#include <stdio.h>

struct CRGBFloatColour {
    float r, g, b, a;
};

struct CShaderRenderManager {
    char pad[0x370];
    CRGBFloatColour fog;   // +0x370
    char pad2[0x3d8 - 0x370 - 16];
    unsigned int flags;    // +0x3d8
    void SetFogColour(CRGBFloatColour* col);
};

void CShaderRenderManager::SetFogColour(CRGBFloatColour* col)
{
    this->fog = *col;
    this->flags |= 0x6000;
}

int main()
{
    CShaderRenderManager m;
    for (int i = 0; i < (int)sizeof(m); ++i) ((char*)&m)[i] = 0;
    m.flags = 0x1;
    CRGBFloatColour c = { 1.0f, 2.0f, 3.0f, 4.0f };
    m.SetFogColour(&c);
    if (m.fog.r == 1.0f && m.fog.g == 2.0f && m.fog.b == 3.0f && m.fog.a == 4.0f
        && m.flags == (0x1u | 0x6000u))
        printf("BEHAVIOUR_OK\n");
    else
        printf("BEHAVIOUR_FAIL\n");
    return 0;
}