// Behaviour fixture for C3DClothPrimitive::SaveBinary and LoadBinary (provider C3DClothPrimitive_Binary_00ac3b00.cpp).
// Collaborators are supplied here (recording): the output writers, the temporary CMemoryDataOutputStream (through
// /alternatename, no stream vtables), CParticleProgram's ctor / WriteToFile / ReadFromFile, the program counted pointer's
// Reset, the handle pool's Allocate / GetPointerFromHandle / Free, the memory input stream ctor / dtor, and the cloth
// memory pool. The input stream is raw storage whose inline-read window (StreamPos +4, StreamSize +8, CurrentSourcePtr
// +0xC, SourceChunkBytesRemaining +0x14) holds the three header ULONGs, and whose vtable slot 4 (ReadCompressed)
// records. Checked: SaveBinary writes PrimitiveIndex, MaterialIndex, the serialised program's length and then the
// program bytes through WriteCompressed. LoadBinary reads the indices into the primitive, allocates the block size from
// the given pool (flags 0; failure returns false before anything else), decompresses into the allocation, parses a new
// program from a memory stream over it with the cloth memory pool, returns true on success; on a parse failure the
// program is released (data and block cleared) and false is returned; the scoped allocation is freed on every path.
#include <stdio.h>
#include <string.h>
#include "fable_3d_cloth_primitive.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_handle_memory_pool.h"

static int g_fail, g_n; static bool g_allocOk, g_readOk;
struct Op { char k; unsigned long u; const void* p; long n; } g_ops[32];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
static void Add(char k, unsigned long u, const void* p, long n) { Op o = { k, u, p, n }; if (g_n < 32) g_ops[g_n] = o; ++g_n; }

static unsigned char g_program[4][0x140]; static int g_programs;
static unsigned char g_block[64];
static unsigned char g_cloth[16];
CMovableResourceMemoryPool* N3DMeshAllocators::ClothMemoryPool = reinterpret_cast<CMovableResourceMemoryPool*>(g_cloth);

void CDataOutputStream::WriteULONG(unsigned long v) { Add('U', v, this, 0); }
void CDataOutputStream::WriteCompressed(const void* d, long n) { Add('C', 0, d, n); }
extern "C" void __fastcall FixOutCtor(void* self) { Add('o', 0, self, 0); }
extern "C" void __fastcall FixOutDtor(void* self) { Add('x', 0, self, 0); }
extern "C" unsigned long __fastcall FixOutLength(const void*) { return 21; }
extern "C" const void* __fastcall FixOutPeek(const void*) { return g_block; }
#pragma comment(linker, "/alternatename:??0CMemoryDataOutputStream@@QAE@XZ=@FixOutCtor@4")
#pragma comment(linker, "/alternatename:??1CMemoryDataOutputStream@@UAE@XZ=@FixOutDtor@4")
#pragma comment(linker, "/alternatename:?GetLength@CMemoryDataOutputStream@@QBEKXZ=@FixOutLength@4")
#pragma comment(linker, "/alternatename:?PeekData@CMemoryDataOutputStream@@QBEPBXXZ=@FixOutPeek@4")

void CParticleProgram::WriteToFile(CDataOutputStream& s) const { Add('W', 0, this, (long)&s); }
extern "C" void* __fastcall FixProgramCtor(void* self) { Add('P', 0, self, 0); return self; }
#pragma comment(linker, "/alternatename:??0CParticleProgram@@QAE@XZ=@FixProgramCtor@4")
bool CParticleProgram::ReadFromFile(CDataInputStream& s, CMovableResourceMemoryPool& pool) { Add('R', 0, &s, (long)&pool); return g_readOk; }
void* operator new(size_t) { return g_program[g_programs++ & 3]; }
void operator delete(void* p) { Add('d', 0, p, 0); }
static void FABLE_FASTCALL DeleteProgram(void* p) { Add('D', 0, p, 0); }
template <> void CCountedPointer<CParticleProgram>::Reset(CParticleProgram* p)
{
    Add('S', 0, p, 0); Data = p; static CCPPointerInfo info(DeleteProgram, 0); info.RefCount = 1; info.Data = p; Info = &info;
}

bool CHandleBasedMemoryPool::Allocate(CScopedMemoryHandle& h, unsigned long size, unsigned long flags)
{
    Add('A', size, this, (long)flags); if (!g_allocOk) return false; h.Pool = this; h.Handle = 77; return true;
}
void* CHandleBasedMemoryPool::GetPointerFromHandle(long handle) { Add('G', handle, this, 0); return g_block; }
void CHandleBasedMemoryPool::Free(long handle) { Add('F', handle, this, 0); }
extern "C" void __fastcall FixInCtor(void* self, int, const void* data, unsigned long size) { Add('I', size, data, (long)self); }
extern "C" void __fastcall FixInDtor(void* self) { Add('i', 0, self, 0); }
#pragma comment(linker, "/alternatename:??0CMemoryDataInputStream@@QAE@PBXK@Z=@FixInCtor@16")
#pragma comment(linker, "/alternatename:??1CMemoryDataInputStream@@UAE@XZ=@FixInDtor@4")
extern "C" void __fastcall FixOverflow(void*, int, void*, long) { CHECK(!"no chunk overflow"); }
#pragma comment(linker, "/alternatename:?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z=@FixOverflow@16")
static void __fastcall ReadCompressed(void* self, int, void* data, long size) { Add('Z', 0, data, size); }
static void __fastcall NotReached(void*, int) { CHECK(!"other stream slot"); }

static unsigned long Word(const void* p, unsigned o) { unsigned long v; memcpy(&v, (const unsigned char*)p + o, 4); return v; }
static void SetU(void* p, unsigned o, unsigned long v) { memcpy((unsigned char*)p + o, &v, 4); }
static int Find(char k) { for (int i = 0; i < g_n; ++i) if (g_ops[i].k == k) return i; return -1; }

int main()
{
    static unsigned char prim[0x14], pool[0x40];
    // SaveBinary.
    SetU(prim, 4, (unsigned long)g_program[3]); SetU(prim, 0xC, 5); SetU(prim, 0x10, 9);
    static unsigned char out[16];
    reinterpret_cast<const C3DClothPrimitive*>(prim)->SaveBinary(*reinterpret_cast<CDataOutputStream*>(out));
    CHECK(g_n == 7);
    CHECK(g_ops[0].k == 'U' && g_ops[0].u == 5 && g_ops[0].p == out && g_ops[1].k == 'U' && g_ops[1].u == 9);
    CHECK(g_ops[2].k == 'o' && g_ops[3].k == 'W' && g_ops[3].p == g_program[3] && g_ops[3].n == (long)g_ops[2].p);
    CHECK(g_ops[4].k == 'U' && g_ops[4].u == 21 && g_ops[5].k == 'C' && g_ops[5].p == g_block && g_ops[5].n == 21);
    CHECK(g_ops[6].k == 'x' && g_ops[6].p == g_ops[2].p);

    // LoadBinary.
    static void* vtable[8] = { (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)ReadCompressed,
                               (void*)NotReached, (void*)NotReached, (void*)NotReached };
    static unsigned long header[3] = { 11, 13, 40 };
    static unsigned char in[0x40];
    for (int pass = 0; pass < 3; ++pass)
    {
        memset(in, 0, sizeof(in)); void* vt = vtable; memcpy(in, &vt, 4);
        SetU(in, 4, 0); SetU(in, 8, 12); SetU(in, 0xC, (unsigned long)header); SetU(in, 0x14, 12);
        memset(prim, 0, sizeof(prim)); g_n = 0;
        g_allocOk = pass != 0; g_readOk = pass == 2;
        bool ok = reinterpret_cast<C3DClothPrimitive*>(prim)->LoadBinary(*reinterpret_cast<CMemoryDataInputStream*>(in),
                                                                          reinterpret_cast<CMovableResourceMemoryPool*>(pool));
        CHECK(Word(prim, 0xC) == 11 && Word(prim, 0x10) == 13);
        int a = Find('A'); CHECK(a >= 0 && g_ops[a].u == 40 && g_ops[a].p == pool && g_ops[a].n == 0);
        if (pass == 0) { CHECK(!ok && g_n == 1); continue; }
        int z = Find('Z'), ic = Find('I'), r = Find('R'), f = Find('F');
        CHECK(z > a && g_ops[z].p == g_block && g_ops[z].n == 40);
        CHECK(ic > z && g_ops[ic].p == g_block && g_ops[ic].u == 40);
        CHECK(Find('S') > ic && Find('P') > ic && r > Find('S') && g_ops[r].p == (const void*)g_ops[ic].n && g_ops[r].n == (long)g_cloth);
        CHECK(f > r && g_ops[f].u == 77 && g_ops[f].p == pool && Find('i') > r);
        if (pass == 1) CHECK(!ok && Word(prim, 4) == 0 && Word(prim, 8) == 0 && Find('D') > r && Find('d') > Find('D'));
        else CHECK(ok && Word(prim, 4) != 0 && Word(prim, 8) != 0);
    }
    if (g_fail) { printf("CLOTH_BINARY_FAIL %d\n", g_fail); return 1; }
    printf("CLOTH_BINARY_OK\n");
    return 0;
}
