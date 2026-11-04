// lib_3d_mesh_file_transformation.cpp, retail 0x00AB9C30-0x00AB9D4F and
// 0x00ABD3E0 (Ego_r C3DMeshFileTransformationChunk): the "TRFM" chunk holding
// a sub-mesh's 3x4 transform (identity when new). Supersedes the stand-in
// landings (..._Methods / ...Retail) at 0x00AB9C30 and 0x00AB9CB0.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AB9C30.
const CCharString C3DMeshFileTransformationChunk::GetChunkInfo() const
{
    return CCharString("");
}

// Retail 00AB9C50.
void C3DMeshFileTransformationChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    Transform = stream.ReadMatrix3x4();
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00AB9CB0.
void C3DMeshFileTransformationChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteMatrix3x4(Transform);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AB9CE0.
CChunkedFileChunk* C3DMeshFileTransformationChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileTransformationChunk);
}

// Retail 00ABD3E0.
const CCharString C3DMeshFileTransformationChunk::GetChunkDescription() const
{
    return CCharString("Transformation Matrix");
}

C3DMeshFileTransformationChunk::C3DMeshFileTransformationChunk()
    : CChunkedFileSuperChunk("TRFM")
{
    Transform.SetIdentity();
}

// Retail 00AB9BB0.
C3DMeshFileTransformationChunk::C3DMeshFileTransformationChunk(const CMatrix3x4& transform)
    : CChunkedFileSuperChunk("TRFM"), Transform(transform)
{
}
