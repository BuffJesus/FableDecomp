#include "engine/CEngineShadowRenderer.h"
void CEngineShadowRenderer::OnReleaseDefaultPoolResources()
{
    UninitialiseTextures();
}
