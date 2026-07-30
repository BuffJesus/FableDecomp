#include <cstdio>

unsigned char g_lengthScaleEnabled = 0;
float g_lengthScaleDivisor = 4.0f;
float g_lengthScaleMultiplier = 3.0f;

float __stdcall LengthScaleConversion(float x)
{
    if (g_lengthScaleEnabled)
        return x / g_lengthScaleDivisor * g_lengthScaleMultiplier;
    return x;
}

int main()
{
    g_lengthScaleEnabled = 0;
    if (LengthScaleConversion(7.5f) != 7.5f)
        return 1;

    g_lengthScaleEnabled = 1;
    g_lengthScaleDivisor = 4.0f;
    g_lengthScaleMultiplier = 3.0f;
    if (LengthScaleConversion(8.0f) != 6.0f)
        return 1;
    if (LengthScaleConversion(100.0f) != 75.0f)
        return 1;

    std::printf("OK_0x0052e4d0\n");
    return 0;
}