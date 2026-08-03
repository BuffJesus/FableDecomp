#include <stdio.h>

float g_terrainHeight = 123.5f;

float __stdcall GetTerrainHeightAt(void* pos)
{
    (void)pos;
    return g_terrainHeight;
}

int main()
{
    void* arg = (void*)0x1000;
    float r = GetTerrainHeightAt(arg);
    if (r != 123.5f) { printf("BAD r=%f\n", r); return 1; }

    g_terrainHeight = -7.25f;
    r = GetTerrainHeightAt(arg);
    if (r != -7.25f) { printf("BAD2 r=%f\n", r); return 1; }

    printf("OK_0x004df3d4 r=%f\n", r);
    return 0;
}