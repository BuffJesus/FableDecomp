// lib_3d_mesh_file_helpers.cpp, retail 0x00AA9910 / 0x00AA9A70 / 0x00AA9E10
// / 0x00AAA0C0 / 0x00AAB180 (Ego_r C3DMeshFileDummyObjectChunk): the "HDMY"
// dummy object, a helper point with a direction, a transform and the
// use-local-origin flag.
// Supersedes the stand-in GetChunkDescription landing at 0x00AAB180.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AA9910.
const CCharString C3DMeshFileDummyObjectChunk::GetChunkInfo() const
{
    return C3DMeshFileHelperPointChunk::GetChunkInfo() + ", Direction: (" + GFFloatToCharString(Direction.X, 6) + ", " +
           GFFloatToCharString(Direction.Y, 6) + ", " + GFFloatToCharString(Direction.Z, 6);
}

// Retail 00AA9A70: a version word, then the dummy's own fields, then the
// helper point.
void C3DMeshFileDummyObjectChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteULONG(3);
    stream.WriteFloat(Direction.X);
    stream.WriteFloat(Direction.Y);
    stream.WriteFloat(Direction.Z);
    stream.WriteMatrix3x4(Transform);
    stream.WriteEBOOL(UseLocalOrigin);
    C3DMeshFileHelperPointChunk::WriteToFile(stream, saver);
}

// Retail 00AA9E60: version 1 files have no transform (identity), versions
// below 3 no local-origin flag.
void C3DMeshFileDummyObjectChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    const unsigned long version = stream.ReadULONG();
    Direction.X = stream.ReadFloat();
    Direction.Y = stream.ReadFloat();
    Direction.Z = stream.ReadFloat();
    if (version >= 2)
        Transform = stream.ReadMatrix3x4();
    else
        Transform.SetIdentity();
    if (version >= 3)
        UseLocalOrigin = stream.ReadEBOOL();
    else
        UseLocalOrigin = false;
    const unsigned long bytes_read = stream.GetPosition() - start;
    C3DMeshFileHelperPointChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00AA9E10.
C3DMeshFileDummyObjectChunk::C3DMeshFileDummyObjectChunk()
    : C3DMeshFileHelperPointChunk("HDMY"), Direction(0.0f, 0.0f, 0.0f), UseLocalOrigin(false)
{
}

// Retail 00AAA0C0.
CChunkedFileChunk* C3DMeshFileDummyObjectChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileDummyObjectChunk);
}

// Retail 00AAB180.
const CCharString C3DMeshFileDummyObjectChunk::GetChunkDescription() const
{
    return CCharString("Dummy Object");
}

// Retail 00AA9A20.
void C3DMeshFileDummyObjectChunk::SetDirection(const C3DVector& direction)
{
    Direction = direction;
}

// Retail 00AA9A40.
void C3DMeshFileDummyObjectChunk::SetTransform(const CMatrix3x4& transform)
{
    Transform = transform;
}

// Retail 00AA9A60.
void C3DMeshFileDummyObjectChunk::SetUseLocalOrigin(bool use_local_origin)
{
    UseLocalOrigin = use_local_origin;
}
