extern unsigned char g_scaleEnabled;
extern float g_gameSpeedDivisor;
extern float g_gameSpeedMultiplier;

float __stdcall Physics_ScaleFloatByGameSpeed(float x)
{
    if (g_scaleEnabled)
    {
        float t = x / g_gameSpeedDivisor;
        return t * g_gameSpeedMultiplier;
    }
    return x;
}