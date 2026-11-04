#include "engine/CEngineShadowRenderer.h"

bool CEngineShadowRenderer::ChangeVideoOptionShadowQuality(long size, float distanceScale)
{
    UserVideoOptionShadowBufferSize = size;
    UserVideoOptionShadowDistanceScale = distanceScale;
    UninitialiseTextures();
    return InitialiseTextures();
}
