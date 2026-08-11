// CEngineInternalPrimitiveRepeatedStaticMesh::CollectStats
// __fastcall forwarder: loads sub-object at this+0x50 into ecx, forwards
// the (map&, statIndex, count) args. The callee receives ecx=sub, edx=map&,
// and (statIndex, count) on the stack -> a __fastcall(self, map&, idx, n).
typedef unsigned long ulong;

struct StatMap; // opaque std::map, passed by reference

struct SubMesh;

// Sub-object's CollectStats: ecx=self, edx=map&, stack=(idx,n)
extern void __fastcall SubMesh_CollectStats(SubMesh* self, StatMap& m, int idx, ulong n);

struct CEngineInternalPrimitiveRepeatedStaticMesh {
    char pad[0x50];
    SubMesh* m50;   // +0x50
    void CollectStats(StatMap& m, int idx, ulong n);
};

void CEngineInternalPrimitiveRepeatedStaticMesh::CollectStats(StatMap& m, int idx, ulong n)
{
    SubMesh_CollectStats(this->m50, m, idx, n);
}