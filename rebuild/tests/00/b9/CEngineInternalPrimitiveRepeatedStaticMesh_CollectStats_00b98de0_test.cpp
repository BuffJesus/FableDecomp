#include <cstdio>
typedef unsigned long ulong;

// Match the opaque decls used by s.cpp
struct StatMap { void* owner; int idx; ulong n; };
struct SubMesh { int marker; };

// The forwarder target: records what it received.
static SubMesh* g_self;
static StatMap* g_map;
static int g_idx;
static ulong g_n;

extern void __fastcall SubMesh_CollectStats(SubMesh* self, StatMap& m, int idx, ulong n);
void __fastcall SubMesh_CollectStats(SubMesh* self, StatMap& m, int idx, ulong n)
{
    g_self = self;
    StatMap* mp = &m;
    g_map = mp;
    g_idx = idx;
    g_n = n;
    m.owner = self;
    m.idx = idx;
    m.n = n;
}

struct CEngineInternalPrimitiveRepeatedStaticMesh {
    char pad[0x50];
    SubMesh* m50;   // +0x50
    void CollectStats(StatMap& m, int idx, ulong n);
};

int main()
{
    CEngineInternalPrimitiveRepeatedStaticMesh obj;
    SubMesh smesh; smesh.marker = 0x1234;
    SubMesh* smp = &smesh;
    obj.m50 = smp;

    StatMap statmap; statmap.owner = 0; statmap.idx = 0; statmap.n = 0;
    obj.CollectStats(statmap, 7, 0xABCDu);

    StatMap* wantmap = &statmap;
    bool ok = (g_self == smp) && (g_map == wantmap) && (g_idx == 7)
              && (g_n == 0xABCDu) && (statmap.owner == smp) && (statmap.idx == 7)
              && (statmap.n == 0xABCDu);
    if (ok) { printf("COLLECTSTATS_FWD_OK\n"); return 0; }
    printf("FAIL self=%p map=%p idx=%d n=%lu\n", (void*)g_self, (void*)g_map, g_idx, g_n);
    return 1;
}