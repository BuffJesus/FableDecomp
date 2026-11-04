// lib_3d_mesh_file_partition.cpp, retail 0x00AC29C0 (Ego_r
// ?CreateNewInstance@C3DMeshFilePartitionChunk@@UBEPAVCChunkedFileChunk@@XZ):
// an empty "MPRT" partition chunk (no partition tree yet).
#include "fable_chunked_file.h"

CChunkedFileChunk* C3DMeshFilePartitionChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFilePartitionChunk);
}

C3DMeshFilePartitionChunk::C3DMeshFilePartitionChunk() : CChunkedFileChunk("MPRT") {}
