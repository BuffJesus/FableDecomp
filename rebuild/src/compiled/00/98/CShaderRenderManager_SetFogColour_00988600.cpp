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