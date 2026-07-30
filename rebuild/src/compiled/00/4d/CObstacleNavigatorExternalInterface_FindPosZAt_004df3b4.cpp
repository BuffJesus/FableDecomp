// CObstacleNavigatorExternalInterface::FindPosZAt @ 0x004df3b4
// Retail: fld dword ptr [g_obstacleNavPosZ]; ret 8
extern float g_obstacleNavPosZ;

float __stdcall FindPosZAt(float a, float b)
{
    return g_obstacleNavPosZ;
}