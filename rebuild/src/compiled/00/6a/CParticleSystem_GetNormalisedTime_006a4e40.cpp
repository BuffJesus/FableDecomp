extern const float g_zeroTime;

struct CParticleSystem {
    char pad[0xb0];
    float totalTime;   /* +0xb0 */
    float elapsed;     /* +0xb4 */
};

float __fastcall CParticleSystem_GetNormalisedTime(const CParticleSystem* self)
{
    if (self->totalTime > g_zeroTime)
        return self->elapsed / self->totalTime;
    return g_zeroTime;
}