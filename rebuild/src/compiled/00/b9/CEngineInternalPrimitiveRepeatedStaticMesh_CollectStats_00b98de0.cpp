#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"
typedef unsigned long ulong;

struct StatMap;
extern void __fastcall SubMesh_CollectStats(C3DMeshInfo* self, StatMap& m, int idx, ulong n);

struct CEngineInternalPrimitiveRepeatedStaticMesh_Methods : CEngineInternalPrimitiveRepeatedStaticMesh {
    void CollectStats(StatMap& m, int idx, ulong n);
};

void CEngineInternalPrimitiveRepeatedStaticMesh_Methods::CollectStats(StatMap& m, int idx, ulong n)
{
    SubMesh_CollectStats(this->MeshInfo, m, idx, n);
}
