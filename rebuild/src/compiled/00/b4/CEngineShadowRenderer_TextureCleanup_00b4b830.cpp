#include "engine/CEngineShadowRenderer.h"
void CEngineShadowRenderer::UninitialiseTextures()
{
    if(TexturesAllocated) {
        RenderTarget.Uninitialise();
        ShadowBufferColourSurface.Uninitialise();
        ShadowBufferDepthSurface.Uninitialise();
        ShadowBufferColourTexture.Uninitialise();
        ShadowBufferTexture.Uninitialise();
        TexturesAllocated = false;
    }
}
