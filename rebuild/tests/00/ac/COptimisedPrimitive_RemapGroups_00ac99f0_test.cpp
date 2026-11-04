// Behaviour fixture for QuadricOptimiserInternals::COptimisedPrimitive::RemapGroups (provider
// COptimisedPrimitive_RemapGroups_00ac99f0.cpp) on raw storage: the primitive's vertex array (+0x10) points at
// retail-stride (0xB8) vertex records. COptimisedVertex::RemapGroups is supplied here and records each call. Checked:
// every vertex is remapped once, in order, with the caller's map; an empty vertex array makes no call.
#include <stdio.h>
#include <string.h>
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;
static int g_fail, g_calls;
static const void* g_this[8]; static const void* g_map[8];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void COptimisedVertex::RemapGroups(const _STL::map<unsigned long, unsigned long>& remap)
{
    if (g_calls < 8) { g_this[g_calls] = this; g_map[g_calls] = &remap; }
    ++g_calls;
}

static unsigned char g_vertices[5][0xB8];
static unsigned char g_prim[0x84];
static unsigned char g_remap[0x10];

int main()
{
    void* begin = g_vertices; void* end = g_vertices + 5;
    memcpy(g_prim + 0x10, &begin, 4); memcpy(g_prim + 0x14, &end, 4); memcpy(g_prim + 0x18, &end, 4);
    COptimisedPrimitive* prim = reinterpret_cast<COptimisedPrimitive*>(g_prim);
    const _STL::map<unsigned long, unsigned long>& remap = *reinterpret_cast<const _STL::map<unsigned long, unsigned long>*>(g_remap);

    prim->RemapGroups(remap);
    CHECK(g_calls == 5);
    for (int i = 0; i < 5; ++i) CHECK(g_this[i] == g_vertices[i] && g_map[i] == g_remap);

    g_calls = 0;
    memcpy(g_prim + 0x14, &begin, 4);
    prim->RemapGroups(remap);
    CHECK(g_calls == 0);

    if (g_fail) { printf("REMAP_GROUPS_FAIL %d\n", g_fail); return 1; }
    printf("REMAP_GROUPS_OK\n");
    return 0;
}
