// lib_chunked_file.cpp, retail 0x00AA75D0-0x00AA76CF and 0x00AA8460 (Ego_r
// CChunkedFileChunk / CChunkedFileSuperChunk base virtuals): an unknown chunk
// describes itself as such and has no info; only super chunks are super chunks;
// a cloned super chunk inherits the registered sub-chunk types.
// Supersedes the stand-in landings (CChunkedFileChunk_Methods, the
// CChunkedFile*_Is*/GetAs* free functions) at these addresses.
#include <string.h>
#include "fable_chunked_file.h"
// Retail expands the counted-pointer copies here (GetSubChunkAt, SaveChunkedFile, ...).
#include "fable_counted_pointer_inline.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_algobase.h"
#include "fable_stlport_vector_impl.h"
#include <new>

// Retail 00AA75D0.
const CCharString CChunkedFileChunk::GetChunkDescription() const
{
    return CCharString("Unknown chunk type");
}

// Retail 00AA75F0.
const CCharString CChunkedFileChunk::GetChunkInfo() const
{
    return CCharString("");
}

// Retail 00AA7610.
int CChunkedFileChunk::IsUnknownType() const
{
    return 0;
}

// Retail 00AA7620.
int CChunkedFileChunk::IsSuperChunk() const
{
    return 0;
}

// Retail 00AA7630 (vtable slot 6).
CChunkedFileSuperChunk* CChunkedFileChunk::GetAsSuperChunk()
{
    return 0;
}

// Retail 00AA7640 (vtable slot 5).
const CChunkedFileSuperChunk* CChunkedFileChunk::GetAsSuperChunk() const
{
    return 0;
}

// Retail 00AA7690.
CChunkedFileChunk* CChunkedFileChunk::InitNewInstance(CChunkedFileChunk* chunk) const
{
    return chunk;
}

// Retail 00AA76A0.
int CChunkedFileSuperChunk::IsSuperChunk() const
{
    return 1;
}

// Retail 00AA76B0.
CChunkedFileSuperChunk* CChunkedFileSuperChunk::GetAsSuperChunk()
{
    return this;
}

// Retail 00AA76C0.
const CChunkedFileSuperChunk* CChunkedFileSuperChunk::GetAsSuperChunk() const
{
    return this;
}

// Retail 00AA8460.
CChunkedFileChunk* CChunkedFileSuperChunk::InitNewInstance(CChunkedFileChunk* chunk) const
{
    chunk->GetAsSuperChunk()->RegisteredSubChunkTypes = RegisteredSubChunkTypes;
    return chunk;
}

// Retail 00AA78C0: each sub-chunk as its 4-character type, a size word
// (patched once the body is written) and the body.
void CChunkedFileSuperChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    for (const CCountedPointer<CChunkedFileChunk>* it = SubChunks.begin(); it != SubChunks.end(); ++it)
    {
        stream.Write(it->Data->ChunkType, 4);
        stream.WriteULONG(0);
        const unsigned long start = stream.GetPosition();
        it->Data->WriteToFile(stream, saver);
        const unsigned long end = stream.GetPosition();
        stream.SetPosition(start - 4);
        stream.WriteULONG(end - start);
        stream.SetPosition(end);
    }
}

// Retail 00AA75C0 (inlined into the scalar deleting destructor 00AA8D50).
CChunkedFileChunk::~CChunkedFileChunk()
{
}

// Retail 00AA8490: sub-chunks are removed first, then both arrays release
// their counted pointers (scalar deleting destructor 00AA9600).
CChunkedFileSuperChunk::~CChunkedFileSuperChunk()
{
    RemoveAllSubChunks();
}


// Retail 00AA7590: the 4-character type, NUL-terminated.
CChunkedFileChunk::CChunkedFileChunk(const char* chunk_type)
    : Parent(0)
{
    memset(ChunkType, 0, sizeof(ChunkType));
    memcpy(ChunkType, chunk_type, 4);
}

// Retail 00AA7740.
int CChunkedFileRawDataChunk::IsUnknownType() const
{
    return 1;
}

// Retail 00AA77A0: the whole body, kept as bytes.
void CChunkedFileRawDataChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader&)
{
    delete[] Data;
    Data = new unsigned char[size];
    ByteSize = size;
    stream.Read(Data, size);
}

// Retail 00AA7830.
void CChunkedFileRawDataChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.Write(Data, ByteSize);
}

// Retail 00AA76D0.
CChunkedFileRawDataChunk::CChunkedFileRawDataChunk(const char* chunk_type)
    : CChunkedFileChunk(chunk_type), Data(0), ByteSize(0)
{
}

// Retail 00AA7850: a raw chunk of the same type (no registered types to copy).
CChunkedFileChunk* CChunkedFileRawDataChunk::CreateNewInstance() const
{
    return new CChunkedFileRawDataChunk(ChunkType);
}

// Inlined into the scalar deleting destructor 00AA8D80.
CChunkedFileRawDataChunk::~CChunkedFileRawDataChunk()
{
    delete[] Data;
}

// Retail 00AA8360.
CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char* chunk_type)
    : CChunkedFileChunk(chunk_type), CurrentChunkIndex(0)
{
}

// Retail 00AA7510: no unknown chunk types unless the caller asks.
CChunkedFileLoader::CChunkedFileLoader(const char* file_type)
{
    memset(SupportedFileType, 0, sizeof(SupportedFileType));
    memcpy(SupportedFileType, file_type, 4);
    ReadUnknownChunkTypes = 0;
}

// Retail 00AA7540.
CChunkedFileLoader::~CChunkedFileLoader()
{
}

// Retail 00AA7550.
CChunkedFileSaver::CChunkedFileSaver(const char* file_type)
{
    memset(SupportedFileType, 0, sizeof(SupportedFileType));
    memcpy(SupportedFileType, file_type, 4);
}

// Retail 00AA7580.
CChunkedFileSaver::~CChunkedFileSaver()
{
}

// Retail 00AA7650: whether `chunk` is this chunk's parent, grandparent, ...
// (a null `chunk` matches the top of the tree).
int CChunkedFileChunk::IsParentRecursive(const CChunkedFileChunk* chunk) const
{
    const CChunkedFileChunk* parent = Parent;
    while (chunk != parent)
    {
        if (!parent)
            return 0;
        parent = parent->Parent;
    }
    return 1;
}

// Retail 00AA7750.
void* CChunkedFileRawDataChunk::GetData() const
{
    return Data;
}

// Retail 00AA7760.
void CChunkedFileRawDataChunk::SetData(const void* data, unsigned long size)
{
    delete[] Data;
    Data = new unsigned char[size];
    ByteSize = size;
    memcpy(Data, data, size);
}

// Retail 00AA78B0.
unsigned long CChunkedFileSuperChunk::GetNoSubChunks() const
{
    return SubChunks.size();
}

// Retail 00AA7950: the registered prototype of a chunk type, if any.
const CChunkedFileChunk* CChunkedFileSuperChunk::FindRegisteredSubChunkType(const char* chunk_type) const
{
    for (const CCountedPointer<CChunkedFileChunk>* it = RegisteredSubChunkTypes.begin(); it != RegisteredSubChunkTypes.end(); ++it)
    {
        if (strcmp(chunk_type, it->Data->ChunkType) == 0)
            return it->Data;
    }
    return 0;
}

// Retail 00AA79C0: ">>>>", the file type, the version, the comment, then the
// root chunk's type, size and body; a mismatch throws.
void CChunkedFileLoader::LoadChunkedFile(CChunkedFile& file, CDataInputStream& stream)
{
    char file_id[4] = {0, 0, 0, 0};
    stream.Read(file_id, 4);
    if (memcmp(file_id, ">>>>", 4) != 0)
        throw CChunkedFileException();
    unsigned long file_type = 0;
    stream.Read(&file_type, 4);
    if (memcmp(&file_type, SupportedFileType, 4) != 0)
        throw CChunkedFileException();
    file.Version = stream.ReadULONG();
    CCharString comment = stream.ReadNullTerminatedString();
    file.FileComment = comment;
    unsigned long root_type;
    stream.Read(&root_type, 4);
    if (memcmp(&root_type, CCountedPointer<CChunkedFileSuperChunk>(file.RootChunk)->ChunkType, 4) != 0)
        throw CChunkedFileException();
    unsigned long root_size = stream.ReadULONG();
    CCountedPointer<CChunkedFileSuperChunk>(file.RootChunk)->ReadFromFile(stream, root_size, *this);
}

// Retail 00AA7C70: ">>>>", the file type, the version, the comment, then the
// root chunk framed like a sub-chunk and a zero terminator word. Returns the
// bytes written.
unsigned long CChunkedFileSaver::SaveChunkedFile(CChunkedFile& file, CDataOutputStream& stream)
{
    const unsigned long file_start = stream.GetPosition();
    stream.Write(">>>>", 4);
    stream.Write(SupportedFileType, 4);
    stream.WriteULONG(file.Version);
    {
        CCharString comment(file.FileComment);
        stream.WriteNullTerminatedString(comment);
    }
    stream.Write(CCountedPointer<CChunkedFileSuperChunk>(file.RootChunk)->ChunkType, 4);
    const unsigned long size_position = stream.GetPosition();
    stream.WriteULONG(0);
    const unsigned long start = stream.GetPosition();
    CCountedPointer<CChunkedFileSuperChunk>(file.RootChunk)->WriteToFile(stream, *this);
    const unsigned long end = stream.GetPosition();
    stream.SetPosition(size_position);
    stream.WriteULONG(end - start);
    stream.SetPosition(end);
    unsigned long terminator = 0;
    stream.Write(&terminator, 4);
    return stream.GetPosition() - file_start;
}

// Retail 00AA7DB0: an empty comment, version 0.
CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root_chunk)
    : RootChunk(root_chunk), Version(0), FileComment("")
{
}

// Retail 00AA7DF0.
CChunkedFile::~CChunkedFile()
{
}

// Retail 00AA7E40.
CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::GetSubChunkAt(unsigned long index)
{
    return SubChunks[index];
}

// Retail 00AA7E70.
const CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::GetSubChunkAt(unsigned long index) const
{
    return SubChunks[index];
}

// STLport's single-element erase is out of line for counted pointers
// (retail 0x00AA91F0).
template <> CCountedPointer<CChunkedFileChunk>* _STL::vector<CCountedPointer<CChunkedFileChunk> >::erase(CCountedPointer<CChunkedFileChunk>* position);

// Retail 00AA7EA0: unknown (raw) chunks never count.
unsigned long CChunkedFileSuperChunk::CountSubChunksOfType(const char* chunk_type) const
{
    unsigned long count = 0;
    for (const CCountedPointer<CChunkedFileChunk>* it = SubChunks.begin(); it != SubChunks.end(); ++it)
    {
        CCountedPointer<CChunkedFileChunk> chunk = *it;
        if (!chunk->IsUnknownType() && strcmp(chunk_type, chunk->ChunkType) == 0)
            ++count;
    }
    return count;
}

// Retail 00AA7F40.
const CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char* chunk_type) const
{
    for (unsigned long index = 0; index < SubChunks.size(); ++index)
    {
        const CCountedPointer<CChunkedFileChunk>& chunk = SubChunks[index];
        if (!chunk->IsUnknownType() && strcmp(chunk_type, chunk->ChunkType) == 0)
        {
            CurrentChunkIndex = index;
            return chunk;
        }
    }
    return CCountedPointer<CChunkedFileChunk>();
}

// Retail 00AA7FF0: resumes after `previous`, found at CurrentChunkIndex or by
// a search.
const CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous) const
{
    unsigned long count = SubChunks.size();
    unsigned long index;
    if (CurrentChunkIndex < count && SubChunks[CurrentChunkIndex].Data == previous.Data)
        index = CurrentChunkIndex;
    else
    {
        for (index = 0; index < count; ++index)
        {
            if (SubChunks[index].Data == previous.Data)
                break;
        }
    }
    const char* chunk_type = previous->ChunkType;
    for (++index; index < count; ++index)
    {
        const CCountedPointer<CChunkedFileChunk>& chunk = SubChunks[index];
        if (!chunk->IsUnknownType() && strcmp(chunk_type, chunk->ChunkType) == 0)
        {
            CurrentChunkIndex = index;
            return chunk;
        }
    }
    return CCountedPointer<CChunkedFileChunk>();
}

// Retail 00AA80E0.
CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char* chunk_type)
{
    for (unsigned long index = 0; index < SubChunks.size(); ++index)
    {
        const CCountedPointer<CChunkedFileChunk>& chunk = SubChunks[index];
        if (!chunk->IsUnknownType() && strcmp(chunk_type, chunk->ChunkType) == 0)
        {
            CurrentChunkIndex = index;
            return chunk;
        }
    }
    return CCountedPointer<CChunkedFileChunk>();
}

// Retail 00AA8190.
CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous)
{
    unsigned long count = SubChunks.size();
    unsigned long index;
    if (CurrentChunkIndex < count && SubChunks[CurrentChunkIndex].Data == previous.Data)
        index = CurrentChunkIndex;
    else
    {
        for (index = 0; index < count; ++index)
        {
            if (SubChunks[index].Data == previous.Data)
                break;
        }
    }
    const char* chunk_type = previous->ChunkType;
    for (++index; index < count; ++index)
    {
        const CCountedPointer<CChunkedFileChunk>& chunk = SubChunks[index];
        if (!chunk->IsUnknownType() && strcmp(chunk_type, chunk->ChunkType) == 0)
        {
            CurrentChunkIndex = index;
            return chunk;
        }
    }
    return CCountedPointer<CChunkedFileChunk>();
}

// Retail 00AA8280.
void CChunkedFileSuperChunk::RemoveSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk)
{
    for (CCountedPointer<CChunkedFileChunk>* it = SubChunks.begin(); it != SubChunks.end(); ++it)
    {
        if (it->Data == chunk.Data)
        {
            it->Data->SetParent(0);
            SubChunks.erase(it);
            return;
        }
    }
}

// Retail 00AA82C0: raw chunks of the type go too.
void CChunkedFileSuperChunk::RemoveSubChunksOfType(const char* chunk_type)
{
    for (unsigned long index = 0; index < SubChunks.size(); ++index)
    {
        if (strcmp(chunk_type, SubChunks[index]->ChunkType) == 0)
        {
            SubChunks[index]->SetParent(0);
            SubChunks.erase(SubChunks.begin() + index);
            --index;
        }
    }
}

// Retail 00AA83B0.
CChunkedFileSuperChunk::CChunkedFileSuperChunk(const CChunkedFileSuperChunk& other)
    : CChunkedFileChunk(other.ChunkType)
{
}

// STLport's range copy and erase for counted pointers: __copy stays out of
// line (retail 0x00AA8F40, folded with other CCountedPointer instantiations),
// erase(first, last) expands into its callers.
namespace _STL
{
    template <>
    CCountedPointer<CChunkedFileChunk>* FABLE_FASTCALL __copy(CCountedPointer<CChunkedFileChunk>* __first, CCountedPointer<CChunkedFileChunk>* __last,
                                                               CCountedPointer<CChunkedFileChunk>* __result, const random_access_iterator_tag&, ptrdiff_t*);

    template <>
    inline CCountedPointer<CChunkedFileChunk>* vector<CCountedPointer<CChunkedFileChunk> >::erase(CCountedPointer<CChunkedFileChunk>* __first,
                                                                                               CCountedPointer<CChunkedFileChunk>* __last)
    {
        CCountedPointer<CChunkedFileChunk>* __i = __copy(__last, this->_M_finish, __first, random_access_iterator_tag(), (ptrdiff_t*)0);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
}

// Retail 00AA8400: orphans every sub-chunk, then empties the array.
void CChunkedFileSuperChunk::RemoveAllSubChunks()
{
    for (unsigned long index = 0; index < SubChunks.size(); ++index)
        SubChunks[index]->SetParent(0);
    SubChunks.clear();
}

// Retail 00AA84F0.
void CChunkedFileSuperChunk::AddSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk)
{
    chunk->SetParent(this);
    SubChunks.push_back(chunk);
}

// Retail 00AA87E0: a prototype that ReadFromFile clones for chunks of its type.
void CChunkedFileSuperChunk::RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>& chunk)
{
    RegisteredSubChunkTypes.push_back(chunk);
}
