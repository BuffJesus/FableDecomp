extern float g_terrainHeight;

float __stdcall GetTerrainHeightAt(void* pos)
{
    (void)pos;
    return g_terrainHeight;
}