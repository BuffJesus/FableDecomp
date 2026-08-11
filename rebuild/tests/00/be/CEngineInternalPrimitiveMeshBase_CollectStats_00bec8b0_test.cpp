#include <cstdio>

typedef unsigned long ulong;

struct StatMap { int dummy; };
enum EEnginePrimitiveStatIndex { EPSI_A = 0 };

struct CEngineInternalPrimitiveMeshBase;

void __fastcall Inner_CollectStats(CEngineInternalPrimitiveMeshBase* self,
                                   StatMap& stats,
                                   EEnginePrimitiveStatIndex idx,
                                   ulong count);

struct CEngineInternalPrimitiveMeshBase {
    char pad[0x50];
    CEngineInternalPrimitiveMeshBase* m_inner;
    void CollectStats(StatMap& stats, EEnginePrimitiveStatIndex idx, ulong count);
};

void CEngineInternalPrimitiveMeshBase::CollectStats(StatMap& stats,
                                                    EEnginePrimitiveStatIndex idx,
                                                    ulong count)
{
    Inner_CollectStats(this->m_inner, stats, idx, count);
}

// Behaviour capture
static CEngineInternalPrimitiveMeshBase* g_self;
static StatMap* g_map;
static int g_idx;
static ulong g_count;

void __fastcall Inner_CollectStats(CEngineInternalPrimitiveMeshBase* self,
                                   StatMap& stats,
                                   EEnginePrimitiveStatIndex idx,
                                   ulong count)
{
    g_self = self; g_map = &stats; g_idx = (int)idx; g_count = count;
}

int main() {
    CEngineInternalPrimitiveMeshBase inner;
    CEngineInternalPrimitiveMeshBase outer;
    outer.m_inner = &inner;
    StatMap m; m.dummy = 7;
    outer.CollectStats(m, (EEnginePrimitiveStatIndex)3, 42u);

    bool ok = (g_self == &inner) && (g_map == &m) && (g_idx == 3) && (g_count == 42u);
    if (ok) { printf("COLLECTSTATS_FWD_OK\n"); return 0; }
    printf("FAIL\n"); return 1;
}