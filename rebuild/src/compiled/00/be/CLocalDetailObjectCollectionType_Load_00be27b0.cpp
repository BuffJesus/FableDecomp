#include "engine/CLocalDetailObjectCollectionType.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CMeshDataBank.h"
#include "fable_engine_internal.h"
#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"

inline long CLocalDetailObjectCollectionType::FindMeshByName(CCharString name) const
{
    if(name.IsEmpty()) return 0;
    return NEngineInternal::GPResourceManager->MeshBanks[0]->FindIndexBySymbol(name);
}

void CLocalDetailObjectCollectionType::Load(CDataInputStream& stream,bool retail)
{
    if(retail) {
        MeshBankIndex=stream.ReadValue<long>();
        ShadowMeshBankIndex=stream.ReadValue<long>();
        ZSpriteMeshBankIndex=stream.ReadValue<long>();
    } else {
        MeshBankIndex=FindMeshByName(stream.ReadNullTerminatedString());
        ShadowMeshBankIndex=FindMeshByName(stream.ReadNullTerminatedString());
        ZSpriteMeshBankIndex=FindMeshByName(stream.ReadNullTerminatedString());
    }
    stream.Read(&PlainDataBlock,sizeof(PlainDataBlock));
    if(MeshBankIndex>0) {
        if(PlainDataBlock.PrimitiveType==1)
            Mesh=NEngineInternal::GPResourceManager->MeshBanks[0]->PeekRepeatedMesh(MeshBankIndex);
        else Mesh=NEngineInternal::GPResourceManager->MeshBanks[0]->PeekStaticMesh(MeshBankIndex);
    }
    if(ShadowMeshBankIndex>0) ShadowMesh=NEngineInternal::GPResourceManager->MeshBanks[0]->PeekStaticMesh(ShadowMeshBankIndex);
    if(ZSpriteMeshBankIndex>0) ZSpriteMesh=NEngineInternal::GPResourceManager->MeshBanks[0]->PeekStaticMesh(ZSpriteMeshBankIndex);
}
