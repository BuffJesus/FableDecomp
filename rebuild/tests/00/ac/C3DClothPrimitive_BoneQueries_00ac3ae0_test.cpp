// Behaviour fixture for C3DClothPrimitive::GetUsedBones / RemapBoneIndices (provider C3DClothPrimitive_BoneQueries_00ac3ae0.cpp).
// The fixture defines recording CParticleProgram methods; the cloth primitive is raw storage laid out like retail
// (its base constructor is not linked) with the counted pointer's data at +0x04. Each query must reach the program
// held by the primitive, with the caller's container, and leave the primitive untouched.
#include <stdio.h>
#include <string.h>
#include "fable_3d_cloth_primitive.h"

static int g_fail, g_used_calls, g_remap_calls;
static const void* g_used_this;
static const void* g_used_arg;
static const void* g_remap_this;
static const void* g_remap_arg;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CParticleProgram::GetUsedBones(CArray<bool>& usedBones) const { ++g_used_calls; g_used_this = this; g_used_arg = &usedBones; }
void CParticleProgram::RemapBoneIndices(_STL::map<unsigned long, unsigned long>& boneRemap) { ++g_remap_calls; g_remap_this = this; g_remap_arg = &boneRemap; }

int main()
{
    char programA[8], programB[8];
    unsigned long raw[5] = { 0x11111111u, 0, 0x22222222u, 3, 9 };
    raw[1] = reinterpret_cast<unsigned long>(programA);
    unsigned long before[5];
    memcpy(before, raw, sizeof(raw));
    C3DClothPrimitive* cloth = reinterpret_cast<C3DClothPrimitive*>(raw);

    CArray<bool>* used = reinterpret_cast<CArray<bool>*>(0x1000);
    _STL::map<unsigned long, unsigned long>* remap = reinterpret_cast<_STL::map<unsigned long, unsigned long>*>(0x2000);
    cloth->GetUsedBones(*used);
    CHECK(g_used_calls == 1 && g_used_this == programA && g_used_arg == used && g_remap_calls == 0);
    cloth->RemapBoneIndices(*remap);
    CHECK(g_remap_calls == 1 && g_remap_this == programA && g_remap_arg == remap && g_used_calls == 1);
    CHECK(memcmp(raw, before, sizeof(raw)) == 0);

    raw[1] = reinterpret_cast<unsigned long>(programB);
    cloth->GetUsedBones(*used);
    cloth->RemapBoneIndices(*remap);
    CHECK(g_used_this == programB && g_remap_this == programB && g_used_calls == 2 && g_remap_calls == 2);

    if (g_fail) { printf("CLOTH_BONE_QUERIES_FAIL %d\n", g_fail); return 1; }
    printf("CLOTH_BONE_QUERIES_OK\n");
    return 0;
}
