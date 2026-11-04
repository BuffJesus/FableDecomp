// Ego_r lib_3d_mesh_file.cpp, retail 0x00A9A870-0x00A9C9C0: the mesh-file
// saver, root chunk, C3DMeshFile and C3DMeshFileLoader. Each
// Register<X>SubChunkType clones a prototype into X's sub-chunk types through
// AddChunkType (FableWin's duplicate-type check is debug-only).
#include "fable_chunked_file.h"
#include "fable_counted_pointer_inline.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_temporary_buffer.h"
#include "fable_lzo.h"
#include <new>
#include "fable_stlport_vector_impl.h"
#include <string.h>

// Retail 00A9A870.
C3DMeshFileSaver::C3DMeshFileSaver()
    : CChunkedFileSaver("3DMF")
{
}

// Retail 00A9A890.
unsigned long C3DMeshFileSaver::SaveMeshFile(C3DMeshFile& data, CDataOutputStream& os)
{
    return SaveChunkedFile(data, os);
}

// Retail 00A9A8A0.
C3DMeshFileRootChunk::C3DMeshFileRootChunk() : CChunkedFileSuperChunk("3DRT") {}

// Retail 00A9A8C0.
CChunkedFileChunk* C3DMeshFileRootChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileRootChunk);
}

// Retail 00A9A900.
C3DMeshFileLoader::~C3DMeshFileLoader()
{
}

// Retail 00A9AD00.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::AddChunkType(const CChunkedFileChunk& chunk_type, CChunkedFileSuperChunk* parent)
{
    CCountedPointer<CChunkedFileChunk> chunk;
    chunk.Reset(chunk_type.CreateNewInstance());
    parent->RegisterSubChunkType(chunk);
    return chunk;
}

// Retail 00A9AD80: a fresh file around a new root chunk.
C3DMeshFile::C3DMeshFile()
    : CChunkedFile(CCountedPointer<CChunkedFileSuperChunk>(new C3DMeshFileRootChunk))
{
    {
        const CCharString comment("Copyright Big Blue Box Studios Ltd.");
        SetFileComment(comment);
    }
    SetVersion(100);
}

// Retail 00A9AE60.
C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root_chunk)
    : CChunkedFile(root_chunk)
{
}

// Retail 00A9B460 / 00A9B550: totals over the sub-meshes (near matches: retail
// keeps the loop pointer in registers and spills `this`; this shape keeps the
// pointer in memory).
unsigned long C3DMeshFileRootChunk::CountVertices() const
{
    unsigned long count = 0;
    CCountedPointer<const C3DMeshFileSubMeshChunk> sub_mesh = FindFirstSubMesh();
    while (sub_mesh.Get())
    {
        count += sub_mesh->CountVertices();
        sub_mesh = FindNextSubMesh(sub_mesh);
    }
    return count;
}

unsigned long C3DMeshFileRootChunk::CountTriangles() const
{
    unsigned long count = 0;
    CCountedPointer<const C3DMeshFileSubMeshChunk> sub_mesh = FindFirstSubMesh();
    while (sub_mesh.Get())
    {
        count += sub_mesh->CountTriangles();
        sub_mesh = FindNextSubMesh(sub_mesh);
    }
    return count;
}

// Retail 00A9B640: a new root of the registered type, loaded into a new file.
CCountedPointer<C3DMeshFile> C3DMeshFileLoader::LoadMeshFile(CDataInputStream& is)
{
    CCountedPointer<C3DMeshFileRootChunk> root(static_cast<C3DMeshFileRootChunk*>(RootChunkType->CreateNewInstance()));
    CCountedPointer<C3DMeshFile> mesh_file(new C3DMeshFile(root));
    const unsigned long position = is.GetPosition();
    is.SetPosition(position);
    LoadChunkedFile(*mesh_file, is);
    return mesh_file;
}

// Retail 00A9B740: ">>>>" marks an uncompressed file; otherwise an LZO block
// prefixed with its uncompressed size, inflated into a temporary buffer.
CCountedPointer<C3DMeshFile> C3DMeshFileLoader::LoadMeshFileCompressed(const void* src, unsigned long src_size, CHandleBasedMemoryPool& memory_pool)
{
    if (memcmp(src, ">>>>", 4) == 0)
    {
        CMemoryDataInputStream is(src, src_size);
        return LoadMeshFile(is);
    }
    const unsigned char* data = static_cast<const unsigned char*>(src);
    const unsigned long uncompressed_size = *reinterpret_cast<const unsigned long*>(data);
    data += 4;
    src_size -= 4;
    CTemporaryBuffer temp_buffer(uncompressed_size + 3, &memory_pool, &GScratchBuffer);
    unsigned char* buffer = static_cast<unsigned char*>(temp_buffer.GetBuffer());
    unsigned long output_size = uncompressed_size;
    lzo1x_decompress_asm_fast(data, src_size, buffer, &output_size, 0);
    CMemoryDataInputStream is(buffer, uncompressed_size);
    CCountedPointer<C3DMeshFile> mesh_file = LoadMeshFile(is);
    return mesh_file;
}

// Retail 00A9B840.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterRootSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    return AddChunkType(clonable_chunk, RootChunkType->GetAsSuperChunk());
}

// Retail 00A9B870.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterSubMeshSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    return AddChunkType(clonable_chunk, SubMeshChunkType->GetAsSuperChunk());
}

// Retail 00A9B8A0.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterPrimitiveSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    return AddChunkType(clonable_chunk, PrimitiveChunkType->GetAsSuperChunk());
}

// Retail 00A9B8D0.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterTrianglesSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    return AddChunkType(clonable_chunk, TrianglesChunkType->GetAsSuperChunk());
}

// Retail 00A9B900.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterGroupSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    return AddChunkType(clonable_chunk, GroupChunkType->GetAsSuperChunk());
}

// Retail 00A9B930: cloth data hangs off both the sub-mesh and primitive cloth chunks.
void C3DMeshFileLoader::RegisterClothSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    AddChunkType(clonable_chunk, SubMeshClothChunkType->GetAsSuperChunk());
    AddChunkType(clonable_chunk, PrimitiveClothChunkType->GetAsSuperChunk());
}

// Retail 00A9B9B0.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterMaterialListSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    return AddChunkType(clonable_chunk, MaterialListChunkType->GetAsSuperChunk());
}

// Retail 00A9B9E0.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterMaterialSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    return AddChunkType(clonable_chunk, MaterialChunkType->GetAsSuperChunk());
}

// Retail 00A9BA10.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterMapSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    return AddChunkType(clonable_chunk, MapChunkType->GetAsSuperChunk());
}

// Retail 00A9BA40.
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::RegisterHelpersSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    return AddChunkType(clonable_chunk, HelpersChunkType->GetAsSuperChunk());
}

// Retail 00A9BA70: the saved file LZO1X-999 compressed, behind its
// uncompressed size; returns the bytes written.
unsigned long C3DMeshFileSaver::SaveMeshFileCompressed(C3DMeshFile& data, CDataOutputStream& os)
{
    CMemoryDataOutputStream temp_os;
    SaveChunkedFile(data, temp_os);
    const unsigned long size = temp_os.GetLength();
    unsigned long compressed_size = size * 65 / 64 + 128;
    CArray<unsigned char> compressed_data(compressed_size);
    CArray<unsigned char> work_mem(LZO1X_999_MEM_COMPRESS);
    lzo1x_999_compress(static_cast<const unsigned char*>(temp_os.PeekData()), size,
                       compressed_data.GetAsCArray(), &compressed_size, work_mem.GetAsCArray());
    const unsigned long start = os.GetPosition();
    os.WriteULONG(size);
    os.Write(compressed_data.GetAsCArray(), compressed_size);
    return os.GetPosition() - start;
}

// Retail 00A9BBE0: the standard mesh-file chunk tree, each type registered
// from a stack prototype.
C3DMeshFileLoader::C3DMeshFileLoader()
    : CChunkedFileLoader("3DMF")
{
    RootChunkType.Reset(new C3DMeshFileRootChunk);
    SubMeshChunkType = RegisterRootSubChunkType(C3DMeshFileSubMeshChunk());
    MaterialListChunkType = RegisterRootSubChunkType(C3DMeshFileMaterialListChunk());
    HelpersChunkType = RegisterRootSubChunkType(C3DMeshFileHelpersChunk());
    TransformationChunkType = RegisterSubMeshSubChunkType(C3DMeshFileTransformationChunk());
    BoneChunkType = RegisterSubMeshSubChunkType(C3DMeshFileBoneChunk());
    PrimitiveChunkType = RegisterSubMeshSubChunkType(C3DMeshFilePrimitiveChunk());
    SubMeshClothChunkType = RegisterSubMeshSubChunkType(C3DMeshFileClothChunk());
    TrianglesChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileTrianglesChunk());
    SmoothingGroupsChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileSmoothingGroupsChunk());
    VerticesChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileVerticesChunk());
    UniqueVerticesChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileUniqueVerticesChunk());
    GroupChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileGroupChunk());
    PrimitiveClothChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileClothChunk());
    RegisterClothSubChunkType(C3DMeshFileGroupChunk());
    MaterialChunkType = RegisterMaterialListSubChunkType(C3DMeshFileMaterialChunk());
    EngineMaterialChunkType = RegisterMaterialListSubChunkType(C3DMeshFileEngineMaterialChunk());
    MaterialExtChunkType = RegisterMaterialSubChunkType(C3DMeshFileMaterialExtChunk());
    MapChunkType = RegisterMaterialSubChunkType(C3DMeshFileMapChunk());
    HelperPointChunkType = RegisterHelpersSubChunkType(C3DMeshFileHelperPointChunk());
    DummyObjectChunkType = RegisterHelpersSubChunkType(C3DMeshFileDummyObjectChunk());
    ConvexVolumeChunkType = RegisterHelpersSubChunkType(C3DMeshFileConvexVolumeChunk());
    MirroredTextureChunkType = RegisterPrimitiveSubChunkType(CMirroredTextureHighlightsChunk());
}

// Retail 00A9C9C0.
const CCharString C3DMeshFileRootChunk::GetChunkDescription() const
{
    return CCharString("3D Mesh File");
}
