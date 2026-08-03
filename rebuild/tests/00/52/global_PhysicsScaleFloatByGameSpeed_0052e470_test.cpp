#include <stdio.h>
#include <math.h>

unsigned char g_scaleEnabled = 0;
float g_gameSpeedDivisor = 4.0f;
float g_gameSpeedMultiplier = 3.0f;

float __stdcall Physics_ScaleFloatByGameSpeed(float x)
{
    if (g_scaleEnabled)
    {
        float t = x / g_gameSpeedDivisor;
        return t * g_gameSpeedMultiplier;
    }
    return x;
}

int main()
{
    g_scaleEnabled = 0;
    float a = Physics_ScaleFloatByGameSpeed(12.5f);
    if (fabs(a - 12.5f) > 0.0001f) { puts("FAIL passthrough"); return 1; }

    g_scaleEnabled = 1;
    float b = Physics_ScaleFloatByGameSpeed(12.0f);
    if (fabs(b - 9.0f) > 0.0001f) { puts("FAIL scaled"); return 1; }

    float c = Physics_ScaleFloatByGameSpeed(8.0f);
    if (fabs(c - 6.0f) > 0.0001f) { puts("FAIL scaled2"); return 1; }

    puts("OK_0x0052e470");
    return 0;
}