// Behaviour fixture for CParticleProgram::ReserveGroups / RemapBoneIndices (provider CParticleProgram_GroupLists_00ae3330.cpp).
// The program's constructor is not linked, so it is raw storage laid out like retail; the fixture supplies the group
// vector's reserve and the private static remap helper, recording their targets. Each wrapper must apply its step to
// SimGroups (+0xFC) and then NonSimGroups (+0x108), passing the caller's count / remap table through.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail, g_reserves, g_remaps;
static const void* g_reserveThis[4];
static size_t g_reserveCount[4];
static const void* g_remapTable[4];
static const void* g_remapGroups[4];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

namespace _STL
{
template <> void vector<C3DGroup2, allocator<C3DGroup2> >::reserve(size_t n)
{
    g_reserveThis[g_reserves & 3] = this; g_reserveCount[g_reserves & 3] = n; ++g_reserves;
}
}
void FABLE_FASTCALL CParticleProgram::RemapBoneIndices(_STL::map<unsigned long, unsigned long>& boneRemap, CArray<C3DGroup2>& groups)
{
    g_remapTable[g_remaps & 3] = &boneRemap; g_remapGroups[g_remaps & 3] = &groups; ++g_remaps;
}

int main()
{
    static unsigned char raw[0x140];
    CParticleProgram* program = reinterpret_cast<CParticleProgram*>(raw);
    program->ReserveGroups(17);
    CHECK(g_reserves == 2 && g_reserveThis[0] == raw + 0xFC && g_reserveThis[1] == raw + 0x108);
    CHECK(g_reserveCount[0] == 17 && g_reserveCount[1] == 17);
    _STL::map<unsigned long, unsigned long>* table = reinterpret_cast<_STL::map<unsigned long, unsigned long>*>(0x4000);
    program->RemapBoneIndices(*table);
    CHECK(g_remaps == 2 && g_remapTable[0] == table && g_remapTable[1] == table);
    CHECK(g_remapGroups[0] == raw + 0xFC && g_remapGroups[1] == raw + 0x108);
    if (g_fail) { printf("PARTICLE_PROGRAM_GROUP_LISTS_FAIL %d\n", g_fail); return 1; }
    printf("PARTICLE_PROGRAM_GROUP_LISTS_OK\n");
    return 0;
}
