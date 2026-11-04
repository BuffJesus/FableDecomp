// NParticleEngine accessors (Ego_r particle_engine.cpp; retail 0x00AEA9C0-0x00AEAB10, the same order as Ego_r):
// rendering switch, tick rate (and its reciprocal), the fade / LOD / draw / spawn distances, the graphic and mesh banks,
// the random seed and the engine singletons. SetGraphicBank / SetMeshBank (0x00AEAA80 / 0x00AEAA90) and their bank
// pointers live in ParticleBankSetters_00aeaa80.cpp (gate_particle_bank_setters). Globals and initial values from Ego_r names and retail .data
// (TickRate 0x01399504 = 15, OneOverTickRate 0x01399508 = 1/15, RenderingEnabled 0x0139950C, MAX_SPAWN_DISTANCE
// 0x01399510 = 48, MAX_DRAW_DISTANCE 0x01399514 = 60, LOD_DISTANCE 0x01399518 = 48; banks / seed 0x013D2E04-0x013D2E10).
#include "fable_particle_engine.h"

static const float FADE_IN_START_DISTANCE = 0.0f;
static const float FADE_IN_END_DISTANCE = 0.0f;
static const float FADE_OUT_START_DISTANCE = 48.0f;
static float MAX_SPAWN_DISTANCE = 48.0f;
static float MAX_DRAW_DISTANCE = 60.0f;
static float LOD_DISTANCE = 48.0f;

namespace NParticleEngine
{
    long TickRate = 15;
    float OneOverTickRate = 1.0f / 15.0f;
    bool RenderingEnabled = true;
    const CGraphicDataBank* PMeshGraphicDataBank = 0;
    unsigned long RandomSeed = 0;

    bool FABLE_FASTCALL IsRenderingEnabled()
    {
        return RenderingEnabled;
    }

    void FABLE_FASTCALL SetTickRate(long tickRate)
    {
        TickRate = tickRate;
        OneOverTickRate = 1.0f / tickRate;
    }

    long FABLE_FASTCALL GetTickRate()
    {
        return TickRate;
    }

    float FABLE_FASTCALL GetOneOverTickRate()
    {
        return OneOverTickRate;
    }

    float FABLE_FASTCALL GetLODDistance()
    {
        return LOD_DISTANCE;
    }

    float FABLE_FASTCALL GetFadeOutStartDistance()
    {
        return FADE_OUT_START_DISTANCE;
    }

    float FABLE_FASTCALL GetFadeInEndDistance()
    {
        return FADE_IN_END_DISTANCE;
    }

    float FABLE_FASTCALL GetFadeInStartDistance()
    {
        return FADE_IN_START_DISTANCE;
    }

    float FABLE_FASTCALL GetMaxDrawDistance()
    {
        return MAX_DRAW_DISTANCE;
    }

    float FABLE_FASTCALL GetMaxSpawnDistance()
    {
        return MAX_SPAWN_DISTANCE;
    }

    void FABLE_FASTCALL SetMeshGraphicBank(const CGraphicDataBank& bank)
    {
        PMeshGraphicDataBank = &bank;
    }

    const CGraphicDataBank& FABLE_FASTCALL GetGraphicBank()
    {
        return *PGraphicDataBank;
    }

    const CMeshDataBank& FABLE_FASTCALL GetMeshBank()
    {
        return *PMeshDataBank;
    }

    void FABLE_FASTCALL SetRandomSeed(unsigned long seed)
    {
        RandomSeed = seed;
    }

    unsigned long& FABLE_FASTCALL GetRandomSeed()
    {
        return RandomSeed;
    }

    CParticleEmitterDatabase& FABLE_FASTCALL GetParticleEmitterDatabase()
    {
        return *PEmitterDatabase;
    }

    CParticleEmitterManager& FABLE_FASTCALL GetParticleEmitterManager()
    {
        return *PEmitterManager;
    }

    CPSCManager& FABLE_FASTCALL GetPSCManager()
    {
        return *PPSCManager;
    }

    CAEnvironmentInteraction& FABLE_FASTCALL GetEnvironmentInteraction()
    {
        return *PEnvironmentInteraction;
    }
}
