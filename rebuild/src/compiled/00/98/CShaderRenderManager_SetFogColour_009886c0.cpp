struct CRGBFloatColour { float r, g, b, a; };

struct CShaderRenderManager {
    char pad0[0x1bc];
    CRGBFloatColour fog;      // +0x1bc
    char pad1[0x3d8 - 0x1bc - 16];
    unsigned int flags;       // +0x3d8

    void SetFogColour(CRGBFloatColour* src);
};

void CShaderRenderManager::SetFogColour(CRGBFloatColour* src)
{
    this->fog = *src;
    this->flags |= 0x20000;
}