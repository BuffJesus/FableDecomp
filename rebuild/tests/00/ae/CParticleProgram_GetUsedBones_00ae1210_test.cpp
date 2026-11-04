// Behaviour fixture for CParticleProgram::GetUsedBones (provider CParticleProgram_GetUsedBones_00ae1210.cpp; the private
// static helper is reached through the public one). The program is raw storage with SimGroups (+0xFC) and NonSimGroups
// (+0x108) pointing at raw groups (bone + blend vector). Every bone of a group with at least one blend must be marked, from
// either list; bones of empty groups and unrelated entries must stay as they were.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static void SetVector(unsigned char* at, void* begin, unsigned bytes)
{
    unsigned char* b = (unsigned char*)begin; unsigned char* e = b + bytes;
    memcpy(at, &b, 4); memcpy(at + 4, &e, 4); memcpy(at + 8, &e, 4);
}

int main()
{
    static unsigned char raw[0x140];
    static unsigned char sim[3][16], nonSim[2][16];
    static unsigned long blends[8];
    long bones[5] = { 2, 4, 6, 1, 7 };
    unsigned blendBytes[5] = { 8, 0, 16, 0, 8 };   // sim: bone 2 used, 4 empty, 6 used; nonsim: 1 empty, 7 used
    unsigned char* g[5] = { sim[0], sim[1], sim[2], nonSim[0], nonSim[1] };
    for (int i = 0; i < 5; ++i) { memcpy(g[i], &bones[i], 4); SetVector(g[i] + 4, blends, blendBytes[i]); }
    SetVector(raw + 0xFC, sim, sizeof(sim));
    SetVector(raw + 0x108, nonSim, sizeof(nonSim));
    const CParticleProgram* program = reinterpret_cast<const CParticleProgram*>(raw);

    static bool used[10];
    unsigned char usedVector[12]; SetVector(usedVector, used, sizeof(used));
    used[0] = true;   // set beforehand: must stay set
    program->GetUsedBones(*reinterpret_cast<CArray<bool>*>(usedVector));
    const bool expect[10] = { true, false, true, false, false, false, true, true, false, false };
    for (int i = 0; i < 10; ++i) CHECK(used[i] == expect[i]);

    if (g_fail) { printf("PARTICLE_USED_BONES_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_USED_BONES_OK\n");
    return 0;
}
