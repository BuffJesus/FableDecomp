// CEngineInternalPrimitiveMeshBase::CollectStats forwarder @ 0x00bec8b0
// Forwards to the member object at this+0x50, which is itself a
// CEngineInternalPrimitiveMeshBase-like object with a CollectStats method.
// Retail passes the map& in edx and (statIndex,count) on the stack -> the
// inner call is __fastcall (ecx=member self, edx=map&).

typedef unsigned long ulong;

struct StatMap { int dummy; };
enum EEnginePrimitiveStatIndex { EPSI_A = 0 };

struct CEngineInternalPrimitiveMeshBase;

// inner CollectStats: __fastcall(member self, map&, statIndex, count)
void __fastcall Inner_CollectStats(CEngineInternalPrimitiveMeshBase* self,
                                   StatMap& stats,
                                   EEnginePrimitiveStatIndex idx,
                                   ulong count);

struct CEngineInternalPrimitiveMeshBase {
    char pad[0x50];
    CEngineInternalPrimitiveMeshBase* m_inner; // +0x50
    void CollectStats(StatMap& stats, EEnginePrimitiveStatIndex idx, ulong count);
};

void CEngineInternalPrimitiveMeshBase::CollectStats(StatMap& stats,
                                                    EEnginePrimitiveStatIndex idx,
                                                    ulong count)
{
    Inner_CollectStats(this->m_inner, stats, idx, count);
}