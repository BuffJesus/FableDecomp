#include <math.h>
#include <stdio.h>
#include <string.h>
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineDefRetail.h"

CEngineDefRetail g_definition;
CEngineDefRetail* g_EngineDef = &g_definition;

int main()
{
    CEngineInternalPrimitiveBase primitive;
    memset(&primitive, 0, sizeof(primitive));
    memset(&g_definition, 0, sizeof(g_definition));
    g_definition.GamePrimitiveDefaultFadeStart = 30.0f;
    g_definition.GamePrimitiveDefaultFadeRangeRatio = 0.5f;

    primitive.SetFadeDistanceWithGamePrimitiveDefaults(-1.0f);
    if (primitive.FadeDistance_FadeBeginDistance != 30.0f) return 1;
    if (primitive.FadeDistance_MaxDrawDistance != 45.0f) return 2;

    primitive.SetFadeDistanceWithGamePrimitiveDefaults(20.0f);
    if (primitive.FadeDistance_FadeBeginDistance != 20.0f) return 3;
    if (primitive.FadeDistance_MaxDrawDistance != 30.0f) return 4;

    printf("FADE_DEFAULTS PASS\n");
    return 0;
}