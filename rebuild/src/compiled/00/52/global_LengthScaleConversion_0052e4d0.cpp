extern unsigned char g_lengthScaleEnabled;
extern float g_lengthScaleDivisor;
extern float g_lengthScaleMultiplier;

float __stdcall LengthScaleConversion(float x)
{
    if (g_lengthScaleEnabled)
    {
        float t = x / g_lengthScaleDivisor;
        return t * g_lengthScaleMultiplier;
    }
    return x;
}