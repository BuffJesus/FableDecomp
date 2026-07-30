#include <stdio.h>

float g_findPosZTheme;

float __stdcall FindPosZAtIncludingThemes(float a, float b)
{
    return g_findPosZTheme;
}

int main()
{
    g_findPosZTheme = 42.5f;
    float r = FindPosZAtIncludingThemes(1.0f, 2.0f);
    if (r != 42.5f) { printf("FAIL got %f\n", r); return 1; }

    g_findPosZTheme = -7.25f;
    r = FindPosZAtIncludingThemes(100.0f, 200.0f);
    if (r != -7.25f) { printf("FAIL got %f\n", r); return 1; }

    printf("OK_0x004df3bd\n");
    return 0;
}