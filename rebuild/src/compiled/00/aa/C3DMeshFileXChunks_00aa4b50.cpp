// Ego_r lib_3d_mesh_file_x.cpp (retail 0x00AA4AD0-0x00AA74E0): the X-format mesh
// chunks and C3DMeshFileXLoader, in one unit as in retail so the chunk
// constructors and destructors expand into the loader constructor.
#include <new>
#include "fable_chunked_file.h"
#include "fable_counted_pointer_inline.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_algobase.h"
#include "fable_mesh_file_x_animated_block.h"

// ---- from C3DMeshFileXLeafChunks_00aa4af0.cpp
// Retail 00AA4AF0.
void C3DMeshFileXTextureIndexChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(Index);
}

// Retail 00AA4B00.
CChunkedFileChunk* C3DMeshFileXTextureIndexChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileXTextureIndexChunk);
}

// Retail 00AA4C00.
void C3DMeshFileXMappingInfoChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(1);
    stream.WriteFloat(AverageTextureStretch);
}

// Retail 00AA4C30.
CChunkedFileChunk* C3DMeshFileXMappingInfoChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileXMappingInfoChunk);
}

// Retail 00AA52B0: the version word is not used.
void C3DMeshFileXMappingInfoChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    stream.ReadValue<unsigned long>();
    AverageTextureStretch = stream.ReadValue<float>();
}

C3DMeshFileXTextureIndexChunk::C3DMeshFileXTextureIndexChunk() : CChunkedFileChunk("XTID"), Index(0) {}
C3DMeshFileXMappingInfoChunk::C3DMeshFileXMappingInfoChunk() : CChunkedFileChunk("XMIF"), AverageTextureStretch(0.0f) {}

// ---- from C3DMeshFileXChunks_00aa4b50.cpp
// Retail 00AA4B50.
void C3DMeshFileXBoundingSphereChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteFloat(Center.X);
    stream.WriteFloat(Center.Y);
    stream.WriteFloat(Center.Z);
    stream.WriteFloat(Radius);
}

// Retail 00AA4CA0.
void C3DMeshFileXAnimatedPrimitiveChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteULONG(BonesPerBlock);
    stream.WriteULONG(BonesPerVertex);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AA4CD0.
CChunkedFileChunk* C3DMeshFileXAnimatedPrimitiveChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileXAnimatedPrimitiveChunk);
}

// Retail 00AA4E40.
void C3DMeshFileXBoundingSphereChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    Center.X = stream.ReadFloat();
    Center.Y = stream.ReadFloat();
    Center.Z = stream.ReadFloat();
    Radius = stream.ReadFloat();
}

// Retail 00AA4FB0.
CChunkedFileChunk* C3DMeshFileXBoundingSphereChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileXBoundingSphereChunk);
}

// Retail 00AA5370.
void C3DMeshFileXAnimatedPrimitiveChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    BonesPerBlock = stream.ReadULONG();
    BonesPerVertex = stream.ReadULONG();
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

C3DMeshFileXBoundingSphereChunk::C3DMeshFileXBoundingSphereChunk()
    : CChunkedFileChunk("XBDS"), Center(0.0f, 0.0f, 0.0f), Radius(0.0f)
{
}

C3DMeshFileXAnimatedPrimitiveChunk::C3DMeshFileXAnimatedPrimitiveChunk()
    : CChunkedFileSuperChunk("XAPR"), BonesPerBlock(0), BonesPerVertex(0)
{
}

// Retail 00AA4B90.
void C3DMeshFileXBoundingBoxChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteFloat(Center.X);
    stream.WriteFloat(Center.Y);
    stream.WriteFloat(Center.Z);
    stream.WriteFloat(Diagonal.X);
    stream.WriteFloat(Diagonal.Y);
    stream.WriteFloat(Diagonal.Z);
}

// Retail 00AA5030.
void C3DMeshFileXBoundingBoxChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    Center.X = stream.ReadFloat();
    Center.Y = stream.ReadFloat();
    Center.Z = stream.ReadFloat();
    Diagonal.X = stream.ReadFloat();
    Diagonal.Y = stream.ReadFloat();
    Diagonal.Z = stream.ReadFloat();
}

// Retail 00AA5250.
CChunkedFileChunk* C3DMeshFileXBoundingBoxChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileXBoundingBoxChunk);
}

C3DMeshFileXBoundingBoxChunk::C3DMeshFileXBoundingBoxChunk()
    : CChunkedFileChunk("XBDB"), Center(0.0f, 0.0f, 0.0f), Diagonal(0.0f, 0.0f, 0.0f)
{
}

// ---- from C3DMeshFileXVertexDataChunk_Methods_00aa4d20.cpp
// Retail 00AA4D20: scale then bias, as floats.
void FABLE_FASTCALL WriteToFile(const CVertexCompressionParams& params, CDataOutputStream& stream)
{
    stream.WriteFloat(params.PosScaleX);
    stream.WriteFloat(params.PosScaleY);
    stream.WriteFloat(params.PosScaleZ);
    stream.WriteFloat(params.PosScaleW);
    stream.WriteFloat(params.PosBiasX);
    stream.WriteFloat(params.PosBiasY);
    stream.WriteFloat(params.PosBiasZ);
    stream.WriteFloat(params.PosBiasW);
}

// Retail 00AA5460.
void FABLE_FASTCALL ReadFromFile(CVertexCompressionParams& params, CDataInputStream& stream)
{
    params.PosScaleX = stream.ReadFloat();
    params.PosScaleY = stream.ReadFloat();
    params.PosScaleZ = stream.ReadFloat();
    params.PosScaleW = stream.ReadFloat();
    params.PosBiasX = stream.ReadFloat();
    params.PosBiasY = stream.ReadFloat();
    params.PosBiasZ = stream.ReadFloat();
    params.PosBiasW = stream.ReadFloat();
}

// Retail 00AA58C0: format version 2.
void C3DMeshFileXVertexDataChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(2);
    stream.WriteULONG(FormatID);
    stream.WriteULONG(VertexCount);
    stream.WriteULONG(VertexSize);
    stream.WriteULONG(InitFlags);
    ::WriteToFile(VBCParams, stream);
    stream.Write(VertexData.GetAsCArray(), VertexData.size());
}

// Retail 00AA59C0.
CChunkedFileChunk* C3DMeshFileXVertexDataChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileXVertexDataChunk);
}

// Retail 00AA66B0: version 1 files carry no compression parameters.
void C3DMeshFileXVertexDataChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    const unsigned long version = stream.ReadULONG();
    FormatID = stream.ReadULONG();
    VertexCount = stream.ReadULONG();
    VertexSize = stream.ReadULONG();
    InitFlags = stream.ReadULONG();
    if (version >= 2)
        ::ReadFromFile(VBCParams, stream);
    const unsigned long byte_count = VertexCount * VertexSize;
    VertexData.resize(byte_count, 0);
    stream.Read(VertexData.GetAsCArray(), byte_count);
}

C3DMeshFileXVertexDataChunk::C3DMeshFileXVertexDataChunk() : CChunkedFileChunk("XVBD") {}

// Retail 00AA6640: the vertices as one block of vertex_count * vertex_size bytes.
void C3DMeshFileXVertexDataChunk::SetVertexData(unsigned long format_id, const void* data, unsigned long vertex_count, unsigned long vertex_size, unsigned long init_flags)
{
    FormatID = format_id;
    VertexCount = vertex_count;
    VertexSize = vertex_size;
    InitFlags = init_flags;
    // FableWin asserts on a byte count past 32 bits; release just returns.
    if ((unsigned __int64)vertex_count * vertex_size > 0xFFFFFFFF)
        return;
    const unsigned long byte_count = vertex_count * vertex_size;
    VertexData.resize(byte_count);
    memcpy(VertexData.GetAsCArray(), data, byte_count);
}

// ---- from C3DMeshFileXUVTangentChunk_Methods_00aa57f0.cpp
// C3DVector has a constructor: STLport erases its vector through the
// element-wise __copy, out of line here (retail 0x0074CA20, a folded
// instantiation), with erase(first, last) expanded into ClearTangents.
namespace _STL
{
    template <>
    C3DVector* FABLE_FASTCALL __copy(C3DVector* __first, C3DVector* __last, C3DVector* __result,
                                     const random_access_iterator_tag&, ptrdiff_t*);

    template <>
    inline C3DVector* vector<C3DVector>::erase(C3DVector* __first, C3DVector* __last)
    {
        C3DVector* __i = __copy(__last, this->_M_finish, __first, random_access_iterator_tag(), (ptrdiff_t*)0);
        this->_M_finish = __i;
        return __first;
    }
}

// Retail 00AA57F0.
void C3DMeshFileXUVTangentChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(1);
    stream.WriteULONG(UTangents.size());
    stream.Write(UTangents.GetAsCArray(), UTangents.size() * sizeof(C3DVector));
    stream.Write(VTangents.GetAsCArray(), VTangents.size() * sizeof(C3DVector));
}

// Retail 00AA5B30.
CChunkedFileChunk* C3DMeshFileXUVTangentChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileXUVTangentChunk);
}

// Retail 00AA6A60: the version word is not used.
void C3DMeshFileXUVTangentChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    stream.ReadULONG();
    const unsigned long count = stream.ReadULONG();
    UTangents.resize(count);
    VTangents.resize(count);
    stream.Read(UTangents.GetAsCArray(), UTangents.size() * sizeof(C3DVector));
    stream.Read(VTangents.GetAsCArray(), VTangents.size() * sizeof(C3DVector));
}

C3DMeshFileXUVTangentChunk::C3DMeshFileXUVTangentChunk() : CChunkedFileChunk("XUVT") {}

void C3DMeshFileXUVTangentChunk::ClearTangents()
{
    UTangents.clear();
    VTangents.clear();
}

void C3DMeshFileXUVTangentChunk::AddTangent(const C3DVector& u_tangent, const C3DVector& v_tangent)
{
    UTangents.push_back(u_tangent);
    VTangents.push_back(v_tangent);
}

// ---- from C3DMeshFileXTriangleStripChunk_Methods_00aa5930.cpp
// Retail 00AA5930.
void C3DMeshFileXTriangleStripChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(StripVertices.size());
    stream.Write(StripVertices.GetAsCArray(), StripVertices.size() * sizeof(unsigned short));
}

// Retail 00AA5A60.
CChunkedFileChunk* C3DMeshFileXTriangleStripChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileXTriangleStripChunk);
}

// Retail 00AA6960.
void C3DMeshFileXTriangleStripChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    const unsigned long count = stream.ReadULONG();
    StripVertices.resize(count, 0);
    stream.Read(StripVertices.GetAsCArray(), StripVertices.size() * sizeof(unsigned short));
}

// Retail 00AA7280.
const CCharString C3DMeshFileXTriangleStripChunk::GetChunkDescription() const
{
    return CCharString("Triangle Strip");
}

C3DMeshFileXTriangleStripChunk::C3DMeshFileXTriangleStripChunk() : CChunkedFileChunk("XTST") {}

void C3DMeshFileXTriangleStripChunk::Initialise(const CArray<unsigned short>& strip_vertices)
{
    StripVertices.resize(strip_vertices.size());
    for (unsigned long i = 0; i < strip_vertices.size(); ++i)
        StripVertices[i] = strip_vertices[i];
}

// ---- from C3DMeshFileXAnimatedPrimitiveBlockChunk_Methods_00aa5b90.cpp
// C3DIndexedBoneWeight has a constructor, so STLport erases its vector
// element by element (__copy expands inline; ClearVertices 0x00AA5BD0).
namespace _STL
{
    template <>
    inline C3DIndexedBoneWeight* vector<C3DIndexedBoneWeight>::erase(C3DIndexedBoneWeight* __first, C3DIndexedBoneWeight* __last)
    {
        C3DIndexedBoneWeight* __i = __copy(__last, this->_M_finish, __first, random_access_iterator_tag(), (ptrdiff_t*)0);
        this->_M_finish = __i;
        return __first;
    }
}

// Retail 00AA5B90: BonesPerVertex is left unset.
C3DMeshFileXAnimatedPrimitiveBlockChunk::C3DMeshFileXAnimatedPrimitiveBlockChunk()
    : CChunkedFileSuperChunk("XAPB")
{
}

// Retail 00AA5BD0.
void C3DMeshFileXAnimatedPrimitiveBlockChunk::ClearVertices()
{
    VertexList.clear();
    BoneWeights.clear();
}

// Retail 00AA5C20.
CChunkedFileChunk* C3DMeshFileXAnimatedPrimitiveBlockChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileXAnimatedPrimitiveBlockChunk);
}

// Retail 00AA5720: the groups, the bones per vertex, the vertex indices, then
// each vertex's bone weights.
void C3DMeshFileXAnimatedPrimitiveBlockChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    const unsigned long group_count = GroupBlock.GetGroupCount();
    stream.WriteULONG(group_count);
    for (unsigned long i = 0; i < group_count; ++i)
        stream.WriteULONG(GroupBlock.GetGroup(i));
    stream.WriteULONG(BonesPerVertex);
    const unsigned long vertex_count = VertexList.size();
    stream.WriteULONG(vertex_count);
    for (unsigned long i = 0; i < vertex_count; ++i)
        stream.WriteULONG(VertexList[i]);
    const unsigned long weight_count = vertex_count * BonesPerVertex;
    for (unsigned long i = 0; i < weight_count; ++i)
    {
        const C3DIndexedBoneWeight& weight = BoneWeights[i];
        stream.WriteULONG(weight.GetBoneIndex());
        stream.WriteULONG((unsigned long)weight.GetWeight());
    }
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AA6C70: as written; the weights pass through float, as in FableWin.
void C3DMeshFileXAnimatedPrimitiveBlockChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    const unsigned long group_count = stream.ReadULONG();
    for (unsigned long i = 0; i < group_count; ++i)
    {
        const unsigned long group = stream.ReadULONG();
        GroupBlock.AddGroup(group);
    }
    BonesPerVertex = stream.ReadULONG();
    const unsigned long vertex_count = stream.ReadULONG();
    VertexList.resize(vertex_count);
    stream.Read(VertexList.GetAsCArray(), vertex_count * sizeof(unsigned long));
    const unsigned long weight_count = vertex_count * BonesPerVertex;
    BoneWeights.resize(weight_count);
    for (unsigned long i = 0; i < weight_count; ++i)
    {
        const unsigned long bone_index = stream.ReadULONG();
        const unsigned long weight = stream.ReadULONG();
        BoneWeights[i].SetBoneIndex(bone_index);
        BoneWeights[i].SetWeight((float)weight);
    }
    const unsigned long read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - read, loader);
}

// Retail 00AA73B0.
const CCharString C3DMeshFileXAnimatedPrimitiveBlockChunk::GetChunkDescription() const
{
    return CCharString("Animated Primitive Block");
}

// One vertex and its BonesPerVertex weights (FableWin asserts the count).
void C3DMeshFileXAnimatedPrimitiveBlockChunk::AddVertex(unsigned long vertex_index, const CArray<C3DIndexedBoneWeight>& weights)
{
    VertexList.push_back(vertex_index);
    for (unsigned long i = 0; i < weights.size(); ++i)
        BoneWeights.push_back(weights[i]);
}

// Retail 00AA5C80: the X chunk types on top of the standard mesh tree; an
// animated primitive holds blocks, each with triangles and a triangle strip.
C3DMeshFileXLoader::C3DMeshFileXLoader()
{
    RegisterRootSubChunkType(C3DMeshFileXBoundingSphereChunk());
    RegisterRootSubChunkType(C3DMeshFileXBoundingBoxChunk());
    RegisterSubMeshSubChunkType(C3DMeshFileXBoundingSphereChunk());
    RegisterSubMeshSubChunkType(C3DMeshFileXBoundingBoxChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXUVTangentChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXMappingInfoChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXBoundingSphereChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXBoundingBoxChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXVertexDataChunk());
    RegisterTrianglesSubChunkType(C3DMeshFileXTriangleStripChunk());
    RegisterMapSubChunkType(C3DMeshFileXTextureIndexChunk());

    CCountedPointer<CChunkedFileChunk> anim_prim_chunk;
    anim_prim_chunk = RegisterPrimitiveSubChunkType(C3DMeshFileXAnimatedPrimitiveChunk());
    CCountedPointer<CChunkedFileChunk> block_chunk(new C3DMeshFileXAnimatedPrimitiveBlockChunk);
    anim_prim_chunk->GetAsSuperChunk()->RegisterSubChunkType(block_chunk);
    CCountedPointer<CChunkedFileChunk> block_tri_chunk(new C3DMeshFileTrianglesChunk);
    block_chunk->GetAsSuperChunk()->RegisterSubChunkType(block_tri_chunk);
    CCountedPointer<CChunkedFileChunk> block_tristrip_chunk(new C3DMeshFileXTriangleStripChunk);
    block_tri_chunk->GetAsSuperChunk()->RegisterSubChunkType(block_tristrip_chunk);
}
