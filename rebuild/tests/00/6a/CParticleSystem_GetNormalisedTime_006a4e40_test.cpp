#include <cstdio>

const float g_zeroTime = 0.0f;

struct CParticleSystem {
    char pad[0xb0];
    float totalTime;
    float elapsed;
};

float __fastcall CParticleSystem_GetNormalisedTime(const CParticleSystem* self)
{
    if (self->totalTime > g_zeroTime)
        return self->elapsed / self->totalTime;
    return g_zeroTime;
}

int main()
{
    CParticleSystem a;
    a.totalTime = 4.0f;
    a.elapsed = 2.0f;
    float r = CParticleSystem_GetNormalisedTime(&a);
    if (r != 0.5f) { std::printf("FAIL div %f\n", r); return 1; }

    CParticleSystem b;
    b.totalTime = 0.0f;
    b.elapsed = 9.0f;
    float z = CParticleSystem_GetNormalisedTime(&b);
    if (z != 0.0f) { std::printf("FAIL zero %f\n", z); return 1; }

    std::printf("CParticleSystem_006a4e40_TEST PASS\n");
    return 0;
}