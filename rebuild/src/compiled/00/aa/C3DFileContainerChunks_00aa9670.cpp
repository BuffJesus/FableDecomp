// Retail container chunks with no data of their own (Ego_r):
//   C3DMeshFileHelpersChunk      "HLPR"  lib_3d_mesh_file_helpers.cpp   0x00AA9670 / 0x00AAACD0
//   C3DMeshFileMaterialListChunk "MTLS"  lib_3d_mesh_file_material.cpp  0x00ABFBB0 / 0x00AC1270
//   C3DAnimFileHelpersChunk      "HLPR"  lib_3d_anim_file_helpers.cpp   0x00AAE0B0 / 0x00AAEA90
// The two "Helpers" descriptions are one folded body (0x00AAACD0 / 0x00AAEA90
// are byte-identical). Supersedes the stand-in GetChunkDescription landings.
// (C3DMeshFileRootChunk "3DRT" lives with the rest of lib_3d_mesh_file.cpp in
// 00/a9/C3DMeshFileLoader_Register_00a9ad00.cpp.)
#include "fable_chunked_file.h"

C3DMeshFileHelpersChunk::C3DMeshFileHelpersChunk() : CChunkedFileSuperChunk("HLPR") {}
CChunkedFileChunk* C3DMeshFileHelpersChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileHelpersChunk);
}
const CCharString C3DMeshFileHelpersChunk::GetChunkDescription() const
{
    return CCharString("Helpers");
}

C3DMeshFileMaterialListChunk::C3DMeshFileMaterialListChunk() : CChunkedFileSuperChunk("MTLS") {}
CChunkedFileChunk* C3DMeshFileMaterialListChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileMaterialListChunk);
}
const CCharString C3DMeshFileMaterialListChunk::GetChunkDescription() const
{
    return CCharString("Material List");
}

C3DAnimFileHelpersChunk::C3DAnimFileHelpersChunk() : CChunkedFileSuperChunk("HLPR") {}
CChunkedFileChunk* C3DAnimFileHelpersChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileHelpersChunk);
}
const CCharString C3DAnimFileHelpersChunk::GetChunkDescription() const
{
    return CCharString("Helpers");
}
