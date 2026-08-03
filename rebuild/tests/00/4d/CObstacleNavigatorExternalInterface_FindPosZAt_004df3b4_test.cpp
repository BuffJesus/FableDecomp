#include <cstdio>

// global that FindPosZAt returns
float g_obstacleNavPosZ = 0.0f;

float __stdcall FindPosZAt(float a, float b)
{
    return g_obstacleNavPosZ;
}

int main()
{
    g_obstacleNavPosZ = 42.5f;
    float r = FindPosZAt(1.0f, 2.0f);
    if (r != 42.5f) { std::printf("BAD %f\n", r); return 1; }

    g_obstacleNavPosZ = -7.25f;
    r = FindPosZAt(9.0f, 9.0f);
    if (r != -7.25f) { std::printf("BAD2 %f\n", r); return 1; }

    std::printf("OK_0x004df3b4 z=%f\n", r);
    return 0;
}