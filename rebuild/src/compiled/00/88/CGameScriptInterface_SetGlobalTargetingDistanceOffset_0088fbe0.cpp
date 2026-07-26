extern float g_globalTargetingDistanceOffset;
float g_globalTargetingDistanceOffset;

void __stdcall CGameScriptInterface_SetGlobalTargetingDistanceOffset(float offset)
{
    g_globalTargetingDistanceOffset = offset;
}