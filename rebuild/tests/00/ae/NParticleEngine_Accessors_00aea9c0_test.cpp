// Behaviour fixture for the NParticleEngine accessors (provider NParticleEngine_Accessors_00aea9c0.cpp). The engine
// singletons' counted pointers and the two bank pointers (defined in other units) are defined here. Checked: the
// retail initial values (rendering on, tick rate 15 with reciprocal 1/15, LOD 48, fade-out start 48, fade-in start/end
// 0, draw 60, spawn 48); SetTickRate stores the rate and its reciprocal; the mesh-graphic bank setter stores the bank (read back from its global);
// the graphic and mesh bank getters return the pointed-to banks; the random seed is set and returned by reference (so
// writes through it stick); the four singleton getters return the counted pointers' objects.
#include <stdio.h>
#include <math.h>
#include "fable_particle_engine.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static unsigned char g_db[0x60], g_manager[0x1018], g_psc[0x10], g_env[0x40], g_bankA[16], g_bankB[16], g_mesh[16];
namespace NParticleEngine
{
    CCountedPointer<CParticleEmitterManager> PEmitterManager;
    CCountedPointer<CPSCManager> PPSCManager;
    CCountedPointer<CAEnvironmentInteraction> PEnvironmentInteraction;
    const CGraphicDataBank* PGraphicDataBank = 0;
    const CMeshDataBank* PMeshDataBank = 0;
    extern const CGraphicDataBank* PMeshGraphicDataBank;
}
CCountedPointer<NParticleEngine::CParticleEmitterDatabase> NParticleEngine::PEmitterDatabase;

int main()
{
    using namespace NParticleEngine;
    CHECK(IsRenderingEnabled());
    CHECK(GetTickRate() == 15 && GetOneOverTickRate() == 1.0f / 15.0f);
    CHECK(GetLODDistance() == 48.0f && GetFadeOutStartDistance() == 48.0f);
    CHECK(GetFadeInEndDistance() == 0.0f && GetFadeInStartDistance() == 0.0f);
    CHECK(GetMaxDrawDistance() == 60.0f && GetMaxSpawnDistance() == 48.0f);

    SetTickRate(30);
    CHECK(GetTickRate() == 30 && fabs(GetOneOverTickRate() - 1.0f / 30.0f) < 1e-9f);
    SetTickRate(4);
    CHECK(GetTickRate() == 4 && GetOneOverTickRate() == 0.25f);

    SetMeshGraphicBank(*reinterpret_cast<const CGraphicDataBank*>(g_bankA));
    CHECK(PMeshGraphicDataBank == (const void*)g_bankA);
    PGraphicDataBank = reinterpret_cast<const CGraphicDataBank*>(g_bankB);
    PMeshDataBank = reinterpret_cast<const CMeshDataBank*>(g_mesh);
    CHECK(&GetGraphicBank() == (const void*)g_bankB && &GetMeshBank() == (const void*)g_mesh);

    SetRandomSeed(1234);
    CHECK(GetRandomSeed() == 1234);
    GetRandomSeed() = 99;
    CHECK(GetRandomSeed() == 99);

    PEmitterDatabase.Data = reinterpret_cast<CParticleEmitterDatabase*>(g_db);
    PEmitterManager.Data = reinterpret_cast<CParticleEmitterManager*>(g_manager);
    PPSCManager.Data = reinterpret_cast<CPSCManager*>(g_psc);
    PEnvironmentInteraction.Data = reinterpret_cast<CAEnvironmentInteraction*>(g_env);
    CHECK(&GetParticleEmitterDatabase() == (void*)g_db && &GetParticleEmitterManager() == (void*)g_manager);
    CHECK(&GetPSCManager() == (void*)g_psc && &GetEnvironmentInteraction() == (void*)g_env);
    PEmitterDatabase.Data = 0; PEmitterManager.Data = 0; PPSCManager.Data = 0; PEnvironmentInteraction.Data = 0;

    if (g_fail) { printf("PARTICLE_ENGINE_ACCESSORS_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_ENGINE_ACCESSORS_OK\n");
    return 0;
}
