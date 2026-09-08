#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"
#include <cstdio>
typedef unsigned long ulong;

struct StatMap { void* owner; int idx; ulong n; };
struct C3DMeshInfo { int marker; };

static C3DMeshInfo* g_self;
static StatMap* g_map;
static int g_idx;
static ulong g_n;

void __fastcall SubMesh_CollectStats(C3DMeshInfo* self, StatMap& m, int idx, ulong n)
{
    g_self = self; g_map = &m; g_idx = idx; g_n = n;
    m.owner = self; m.idx = idx; m.n = n;
}

struct CEngineInternalPrimitiveRepeatedStaticMesh_Methods : CEngineInternalPrimitiveRepeatedStaticMesh {
    void CollectStats(StatMap& m, int idx, ulong n);
};

int main()
{
    CEngineInternalPrimitiveRepeatedStaticMesh_Methods obj;
    C3DMeshInfo smesh; smesh.marker = 0x1234;
    C3DMeshInfo* smp = &smesh; obj.MeshInfo = smp;
    StatMap statmap; statmap.owner = 0; statmap.idx = 0; statmap.n = 0;
    obj.CollectStats(statmap, 7, 0xABCDu);
    bool ok = g_self == smp && g_map == &statmap && g_idx == 7 &&
              g_n == 0xABCDu && statmap.owner == smp && statmap.idx == 7 &&
              statmap.n == 0xABCDu;
    if (ok) { printf("COLLECTSTATS_FWD_OK\n"); return 0; }
    printf("FAIL self=%p map=%p idx=%d n=%lu\n", (void*)g_self, (void*)g_map, g_idx, g_n);
    return 1;
}
