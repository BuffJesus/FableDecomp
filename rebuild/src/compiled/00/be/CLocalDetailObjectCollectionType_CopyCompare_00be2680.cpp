#include <string.h>
#include "engine/CLocalDetailObjectCollectionType.h"

bool CLocalDetailObjectCollectionType::IsEquivalent(const CLocalDetailObjectCollectionType& other) const
{
    return MeshBankIndex==other.MeshBankIndex
        && ZSpriteMeshBankIndex==other.ZSpriteMeshBankIndex
        && ShadowMeshBankIndex==other.ShadowMeshBankIndex
        && memcmp(&PlainDataBlock,&other.PlainDataBlock,sizeof(PlainDataBlock))==0;
}

CLocalDetailObjectCollectionType::CLocalDetailObjectCollectionType(const CLocalDetailObjectCollectionType& other)
{
    MeshBankIndex=other.MeshBankIndex;
    ShadowMeshBankIndex=other.ShadowMeshBankIndex;
    ZSpriteMeshBankIndex=other.ZSpriteMeshBankIndex;
    MeshPolyCount=other.MeshPolyCount;
    Mesh=other.Mesh;
    ShadowMesh=other.ShadowMesh;
    ZSpriteMesh=other.ZSpriteMesh;
    memcpy(&PlainDataBlock,&other.PlainDataBlock,sizeof(PlainDataBlock));
}
