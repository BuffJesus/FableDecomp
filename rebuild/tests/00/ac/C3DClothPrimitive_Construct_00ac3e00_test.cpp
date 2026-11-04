// Behaviour fixture for C3DClothPrimitive::C3DClothPrimitive(const C3DMeshFileClothChunk&) and
// CCountedPointer<CParticleProgram>::Reset (provider C3DClothPrimitive_Construct_00ac3e00.cpp). The fixture supplies
// every collaborator the constructor calls - the particle program (ctor/ReadFromFile/ReserveGroups/AddGroup), the
// group conversion, the memory input stream, the chunk's group walk and sub-chunk count, the cloth memory pool and the
// base class - and records what they receive. The chunk is raw storage laid out like retail (MaterialIndex +0x30,
// CompiledCode vector +0x38). Checked: a fresh program owned through one shared block; the stream spans exactly the
// compiled code (or the vector object itself when empty); ReadFromFile gets that stream and the cloth pool; the
// material index is copied; ReserveGroups gets the GROUP sub-chunk count; every group is converted and added in order;
// and no group pointer reference is leaked.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_counted_pointer_inline.h"
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_3d_group2.h"
#include "fable_3d_cloth_primitive.h"
// ~C3DClothPrimitive lives in C3DClothPrimitive_Destructor_00ac3b80.cpp; the provider's vtable references it.
C3DClothPrimitive::~C3DClothPrimitive() {}

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

// --- recorded collaborators -----------------------------------------------------------------------------------------
static int g_programs, g_reads, g_reserved = -1, g_added, g_converted;
static const void* g_programThis;
static const void* g_streamData;
static unsigned long g_streamSize;
static const void* g_readStream;
static const void* g_readPool;
static const void* g_addedThis;
static const void* g_convertedFrom[8];
static const void* g_addedFrom[8];
static const char* g_countedType;

// The program constructor is supplied by mangled name: defining it here would construct every member (and emit their
// vtables) now that CParticleProgram's layout is modelled.
extern "C" void* __fastcall FakeProgramCtor(void* self) { ++g_programs; g_programThis = self; return self; }
#pragma comment(linker, "/alternatename:??0CParticleProgram@@QAE@XZ=@FakeProgramCtor@4")
bool CParticleProgram::ReadFromFile(CDataInputStream& stream, CMovableResourceMemoryPool& pool)
{
    ++g_reads; g_readStream = &stream; g_readPool = &pool; return true;
}
void CParticleProgram::ReserveGroups(unsigned long count) { g_reserved = (int)count; }
void CParticleProgram::AddGroup(const C3DGroup2& group)
{
    g_addedThis = this;
    // The group's BoneIndex (first member) carries the source chunk address set by the fake conversion below.
    g_addedFrom[g_added++ & 7] = *reinterpret_cast<const void* const*>(&group);
}
void CParticleProgram::RelocateData(long, void*, void*, long) {}
bool CMovableResource::CanRelocateData(long) const { return false; }
unsigned long CMovableResource::Verify() const { return 0; }

C3DGroup2::C3DGroup2(const C3DMeshFileGroupChunk& chunk)
{
    g_convertedFrom[g_converted++ & 7] = &chunk;
    BoneIndex = (long)&chunk;
}

// The stream and the base class are supplied by mangled name (defining their members here would emit their vtables):
// __thiscall (this, args..., callee pops) is call-compatible with __fastcall (this, edx, args...).
static int g_streams, g_bases;
extern "C" void* __fastcall FakeStreamCtor(void* self, int, const void* data, unsigned long size)
{
    ++g_streams; g_streamData = data; g_streamSize = size; return self;
}
extern "C" void __fastcall FakeStreamDtor(void*) { --g_streams; }
extern "C" void* __fastcall FakeBaseCtor(void* self) { ++g_bases; return self; }
extern "C" void __fastcall FakeBaseDtor(void*) { --g_bases; }
#pragma comment(linker, "/alternatename:??0CMemoryDataInputStream@@QAE@PBXK@Z=@FakeStreamCtor@16")
#pragma comment(linker, "/alternatename:??1CMemoryDataInputStream@@UAE@XZ=@FakeStreamDtor@4")
#pragma comment(linker, "/alternatename:??0CBaseClassNonCopyable@@QAE@XZ=@FakeBaseCtor@4")
#pragma comment(linker, "/alternatename:??1CBaseClassNonCopyable@@UAE@XZ=@FakeBaseDtor@4")

static unsigned char g_groups[3][0x40];
static int g_live;   // group pointers handed out and not yet released
static void FABLE_FASTCALL ReleaseGroup(void*) { --g_live; }
static CCountedPointer<const C3DMeshFileGroupChunk> MakeGroup(int i)
{
    CCountedPointer<const C3DMeshFileGroupChunk> p;
    p.Data = reinterpret_cast<const C3DMeshFileGroupChunk*>(g_groups[i]);
    p.Info = new CCPPointerInfo(&ReleaseGroup, g_groups[i]);
    ++g_live;
    return p;
}
static int g_groupCount;
CCountedPointer<const C3DMeshFileGroupChunk> C3DMeshFileClothChunk::FindFirstGroup() const
{
    return g_groupCount ? MakeGroup(0) : CCountedPointer<const C3DMeshFileGroupChunk>();
}
CCountedPointer<const C3DMeshFileGroupChunk> C3DMeshFileClothChunk::FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous) const
{
    for (int i = 0; i + 1 < g_groupCount; ++i)
        if (previous.Data == reinterpret_cast<const C3DMeshFileGroupChunk*>(g_groups[i])) return MakeGroup(i + 1);
    return CCountedPointer<const C3DMeshFileGroupChunk>();
}
unsigned long CChunkedFileSuperChunk::CountSubChunksOfType(const char* type) const { g_countedType = type; return (unsigned long)g_groupCount; }

static unsigned char g_pool[16];
CMovableResourceMemoryPool* N3DMeshAllocators::ClothMemoryPool = reinterpret_cast<CMovableResourceMemoryPool*>(g_pool);

// --- driver ----------------------------------------------------------------------------------------------------------
static void Reset()
{
    g_programs = g_reads = g_added = g_converted = 0; g_reserved = -1;
    g_programThis = g_streamData = g_readStream = g_readPool = g_addedThis = 0; g_streamSize = 0; g_countedType = 0;
}

static void Run(int groups, unsigned long codeSize)
{
    Reset(); g_groupCount = groups;
    static unsigned char chunk[0x60];
    memset(chunk, 0, sizeof(chunk));
    long material = 7 + groups;
    memcpy(chunk + 0x30, &material, 4);
    static char code[32];
    char* begin = codeSize ? code : 0;
    char* end = codeSize ? code + codeSize : 0;
    memcpy(chunk + 0x38, &begin, 4); memcpy(chunk + 0x3C, &end, 4); memcpy(chunk + 0x40, &end, 4);

    static unsigned char storage[sizeof(C3DClothPrimitive)];
    C3DClothPrimitive* cloth = new (storage) C3DClothPrimitive(*reinterpret_cast<const C3DMeshFileClothChunk*>(chunk));

    CHECK(g_programs == 1 && cloth->ParticleProgram.Data == g_programThis);
    CHECK(cloth->ParticleProgram.Info != 0 && cloth->ParticleProgram.Info->RefCount == 1);
    CHECK(g_streamData == (codeSize ? (const void*)code : (const void*)(chunk + 0x38)) && g_streamSize == codeSize);
    CHECK(g_reads == 1 && g_readPool == g_pool && g_readStream != 0);
    CHECK(cloth->MaterialIndex == (unsigned long)material);
    CHECK(g_reserved == groups && g_countedType != 0 && strcmp(g_countedType, C3DMeshFileGroupChunk::GetStaticChunkType()) == 0);
    CHECK(g_converted == groups && g_added == groups);
    for (int i = 0; i < groups; ++i)
        CHECK(g_convertedFrom[i] == g_groups[i] && g_addedFrom[i] == g_groups[i]);
    if (groups) CHECK(g_addedThis == g_programThis);
    CHECK(g_live == 0);
    CHECK(g_streams == 0 && g_bases == 1);   // the stream is destroyed on exit; the base stays constructed
    g_bases = 0;
}

int main()
{
    Run(0, 0);
    Run(1, 12);
    Run(3, 20);
    if (g_fail) { printf("CLOTH_PRIMITIVE_CONSTRUCT_FAIL %d\n", g_fail); return 1; }
    printf("CLOTH_PRIMITIVE_CONSTRUCT_OK\n");
    return 0;
}
