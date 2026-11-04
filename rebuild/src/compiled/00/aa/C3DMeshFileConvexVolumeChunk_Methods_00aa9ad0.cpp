// lib_3d_mesh_file_helpers.cpp, retail 0x00AA9AD0 / 0x00AAA9A0 / 0x00AAAA60
// / 0x00AAB6B0 (Ego_r C3DMeshFileConvexVolumeChunk): the "HCVL" helper
// convex volume (a name and its bounding planes). The writer does not write
// sub-chunks. Supersedes
// the stand-in GetChunkDescription landing at 0x00AAB6B0.
#include <new>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"

// As for the vertex arrays: erase is out of line (retail 0x00AAB500) and
// resize is inlined here (with _M_fill_insert 0x00AAB570).
namespace _STL {
template<> CPlane* vector<CPlane>::erase(CPlane* __first, CPlane* __last);
template<> inline void vector<CPlane>::resize(size_t __new_size, const CPlane& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}
}

// Retail 00AA9AD0.
const CCharString C3DMeshFileConvexVolumeChunk::GetChunkInfo() const
{
    return CCharString("Name: ") + Name;
}

// Retail 00AAA9A0: a version word, the name, then the planes.
void C3DMeshFileConvexVolumeChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(1);
    stream.WriteNullTerminatedString(Name);
    const unsigned long count = Planes.size();
    stream.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i)
    {
        const CPlane& plane = Planes[i];
        stream.Write3DVector(plane.Normal);
        stream.WriteFloat(plane.Distance);
    }
}

// Retail 00AAAA60.
CChunkedFileChunk* C3DMeshFileConvexVolumeChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileConvexVolumeChunk);
}

// Retail 00AAAB10: the version word is not used.
void C3DMeshFileConvexVolumeChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    stream.ReadULONG();
    Name = stream.ReadNullTerminatedString();
    const unsigned long count = stream.ReadULONG();
    Planes.resize(count);
    for (unsigned long i = 0; i < count; ++i)
    {
        CPlane& plane = Planes[i];
        plane.Normal = stream.Read3DVector();
        plane.Distance = stream.ReadFloat();
    }
}

// Retail 00AAB6B0.
const CCharString C3DMeshFileConvexVolumeChunk::GetChunkDescription() const
{
    return CCharString("Convex Volume");
}

C3DMeshFileConvexVolumeChunk::C3DMeshFileConvexVolumeChunk() : CChunkedFileSuperChunk("HCVL") {}

// Retail 00AA9B10.
void C3DMeshFileConvexVolumeChunk::SetName(const CCharString& name)
{
    Name = name;
}

// Retail 00AAAA40.
void C3DMeshFileConvexVolumeChunk::ClearPlanes()
{
    Planes.clear();
}

void C3DMeshFileConvexVolumeChunk::AddPlane(const CPlane& plane)
{
    Planes.push_back(plane);
}
