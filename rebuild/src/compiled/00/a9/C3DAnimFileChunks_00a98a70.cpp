// Anim-file chunks (Ego_r): "ANRT" anim root (lib_3d_anim_file.cpp, retail
// 0x00A98A70 / 0x00A98AB0 / 0x00A98AF0 / 0x00A99E30: loop flag and duration
// ahead of the sub-chunks) and "XALO" allocation size (lib_3d_anim_file_x.cpp,
// retail 0x00AA4500 / 0x00AA4530 / 0x00AA4970; its ReadFromFile 0x00AA4570 is
// a near match left out, see C3DMeshFileXLeafChunks_00aa4af0.cpp). Supersedes the stand-in landings at 0x00A99E30,
// 0x00AA4500 and 0x00AA4970.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_counted_pointer_inline.h"
#include "fable_temporary_buffer.h"
#include "fable_lzo.h"
#include <new>
#include "fable_stlport_vector_impl.h"
#include <string.h>

// Retail 00A98A70.
void C3DAnimFileRootChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteEBOOL(LoopFlag);
    stream.WriteFloat(Duration);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00A98AB0.
CChunkedFileChunk* C3DAnimFileRootChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileRootChunk);
}

// Retail 00A98AF0.
void C3DAnimFileRootChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    LoopFlag = stream.ReadEBOOL();
    Duration = stream.ReadFloat();
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00A99E30.
const CCharString C3DAnimFileRootChunk::GetChunkDescription() const
{
    return CCharString("3D Sequence File");
}

C3DAnimFileRootChunk::C3DAnimFileRootChunk() : CChunkedFileSuperChunk("ANRT") {}

// Retail 00AA4500.
void C3DAnimFileXAllocationSizeChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(1);
    stream.WriteULONG(AllocSize);
}

// Retail 00AA4530.
CChunkedFileChunk* C3DAnimFileXAllocationSizeChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileXAllocationSizeChunk);
}

// Retail 00AA4970.
const CCharString C3DAnimFileXAllocationSizeChunk::GetChunkDescription() const
{
    return CCharString("Allocation Size Helper Chunk");
}


// Retail 00A98A50.
void C3DAnimFileRootChunk::SetCyclic(bool cyclic)
{
    LoopFlag = cyclic;
}

// Retail 00A98A60.
void C3DAnimFileRootChunk::SetDuration(float duration)
{
    Duration = duration;
}

// Retail 00A98A00.
C3DAnimFileSaver::C3DAnimFileSaver()
    : CChunkedFileSaver("3DAF")
{
}

// Retail 00A98A20.
unsigned long C3DAnimFileSaver::SaveAnimFile(C3DAnimFile& file, CDataOutputStream& stream)
{
    return SaveChunkedFile(file, stream);
}

// Retail 00A98BE0 / 00A98C80 / 00A98D20 / 00A98DC0: clone the prototype and
// register the clone with its parent chunk type.
CCountedPointer<CChunkedFileChunk> C3DAnimFileLoader::RegisterRootSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    CCountedPointer<CChunkedFileChunk> chunk(clonable_chunk.CreateNewInstance());
    RootChunkType->GetAsSuperChunk()->RegisterSubChunkType(chunk);
    return chunk;
}

CCountedPointer<CChunkedFileChunk> C3DAnimFileLoader::RegisterObjectSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    CCountedPointer<CChunkedFileChunk> chunk(clonable_chunk.CreateNewInstance());
    ObjectChunkType->GetAsSuperChunk()->RegisterSubChunkType(chunk);
    return chunk;
}

CCountedPointer<CChunkedFileChunk> C3DAnimFileLoader::RegisterHelpersSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    CCountedPointer<CChunkedFileChunk> chunk(clonable_chunk.CreateNewInstance());
    HelpersChunkType->GetAsSuperChunk()->RegisterSubChunkType(chunk);
    return chunk;
}

CCountedPointer<CChunkedFileChunk> C3DAnimFileLoader::RegisterMovementVectorSubChunkType(const CChunkedFileChunk& clonable_chunk)
{
    CCountedPointer<CChunkedFileChunk> chunk(clonable_chunk.CreateNewInstance());
    MovementVectorChunkType->GetAsSuperChunk()->RegisterSubChunkType(chunk);
    return chunk;
}

// Retail 00A98E60: a fresh file around a new root chunk.
C3DAnimFile::C3DAnimFile()
    : CChunkedFile(CCountedPointer<CChunkedFileSuperChunk>(new C3DAnimFileRootChunk))
{
    {
        const CCharString comment("Copyright Big Blue Box Studios Ltd.");
        SetFileComment(comment);
    }
    SetVersion(100);
}

// Retail 00A98F40.
C3DAnimFile::C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root_chunk)
    : CChunkedFile(root_chunk)
{
}

// Retail 00A99420: a new root of the registered type, loaded into a new file.
CCountedPointer<C3DAnimFile> C3DAnimFileLoader::LoadAnimFile(CDataInputStream& is)
{
    CCountedPointer<C3DAnimFileRootChunk> root(static_cast<C3DAnimFileRootChunk*>(RootChunkType->CreateNewInstance()));
    CCountedPointer<C3DAnimFile> anim_file(new C3DAnimFile(root));
    LoadChunkedFile(*anim_file, is);
    return anim_file;
}

// Retail 00A99510: ">>>>" marks an uncompressed file; otherwise an LZO block
// prefixed with its uncompressed size, inflated into a temporary buffer.
CCountedPointer<C3DAnimFile> C3DAnimFileLoader::LoadAnimFileCompressed(const void* src, unsigned long src_size, CHandleBasedMemoryPool& memory_pool)
{
    if (memcmp(src, ">>>>", 4) == 0)
    {
        CMemoryDataInputStream is(src, src_size);
        return LoadAnimFile(is);
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
    CCountedPointer<C3DAnimFile> anim_file = LoadAnimFile(is);
    return anim_file;
}

// Retail 00A99610: the first object's first sequence, from its first to its
// last sample.
float C3DAnimFileRootChunk::GetNonLoopingDuration() const
{
    CCountedPointer<const C3DAnimFileObjectChunk> object = FindFirstObject();
    CCountedPointer<const C3DAnimFileSequenceChunk> sequence = object->FindFirstSequence();
    const float duration = ((float)sequence->GetMatrixCount() - 1.0f) / sequence->GetSamplesPerSecond();
    return duration;
}

// Retail 00A996D0.
C3DAnimFileLoader::~C3DAnimFileLoader()
{
}

// Retail 00A99840: the saved file LZO1X-999 compressed, behind its
// uncompressed size; returns the bytes written.
unsigned long C3DAnimFileSaver::SaveAnimFileCompressed(C3DAnimFile& data, CDataOutputStream& os)
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

// Retail 00A999B0: the standard anim-file chunk tree, each type registered
// from a stack prototype.
C3DAnimFileLoader::C3DAnimFileLoader()
    : CChunkedFileLoader("3DAF")
{
    RootChunkType.Reset(new C3DAnimFileRootChunk);
    ObjectChunkType = RegisterRootSubChunkType(C3DAnimFileObjectChunk());
    HelpersChunkType = RegisterRootSubChunkType(C3DAnimFileHelpersChunk());
    SequenceChunkType = RegisterObjectSubChunkType(C3DAnimFileSequenceChunk());
    BoneMaskChunkType = RegisterObjectSubChunkType(C3DAnimFileBoneMaskChunk());
    MovementVectorChunkType = RegisterHelpersSubChunkType(C3DAnimFileMovementVectorChunk());
    TimingEventChunkType = RegisterHelpersSubChunkType(C3DAnimFileTimingEventChunk());
    MovementTrackChunkType = RegisterMovementVectorSubChunkType(C3DAnimFileSequenceChunk());
}
