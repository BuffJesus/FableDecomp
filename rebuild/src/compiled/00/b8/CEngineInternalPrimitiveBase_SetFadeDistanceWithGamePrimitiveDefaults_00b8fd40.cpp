#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineDefRetail.h"

extern CEngineDefRetail* g_EngineDef;

void CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults(
    float fadeBeginDistance)
{
    if (fadeBeginDistance <= 0.0f)
        fadeBeginDistance = g_EngineDef->GamePrimitiveDefaultFadeStart;

    FadeDistance_FadeBeginDistance = fadeBeginDistance;
    FadeDistance_MaxDrawDistance =
        (g_EngineDef->GamePrimitiveDefaultFadeRangeRatio + 1.0f) *
        fadeBeginDistance;
}