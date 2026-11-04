// Behaviour fixture for CParticleProgram::OptimiseConstraints (provider CParticleProgram_OptimiseConstraints_00adfdc0.cpp),
// reached through its mangled name (private). Every stream member the provider calls is replaced through /alternatename
// (no stream vtables are emitted): memory output streams are buffers keyed by object address; the input stream
// constructor points the inline-read fields (StreamPos +4, StreamSize +8, CurrentSourcePtr +0xC, SourceChunkBytesRemaining
// +0x14) at the source buffer, and CopyTo moves bytes from that window. The program's Constraints (+0x114) is raw storage
// whose vtable slot 4 (Write) appends to its buffer. Checked: runs of same-type records merge (counts and payload sizes
// summed, payloads concatenated in order), REPEAT and END_REPEAT records never merge, a change of type closes a run,
// records with a zero count do not open a run that is written (retail's `if (runCount)`), and Constraints is cleared then
// rewritten with the result; an empty program stays empty.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct Buf { const void* owner; unsigned char data[512]; unsigned long len; int clears; };
static Buf g_bufs[8]; static int g_nbufs;
static Buf* Find(const void* o) { for (int i = 0; i < g_nbufs; ++i) if (g_bufs[i].owner == o) return &g_bufs[i]; Buf* b = &g_bufs[g_nbufs++]; memset(b, 0, sizeof(*b)); b->owner = o; return b; }
static void Append(const void* o, const void* d, unsigned long n) { Buf* b = Find(o); memcpy(b->data + b->len, d, n); b->len += n; }
static void SetU(void* o, unsigned off, unsigned long v) { memcpy((unsigned char*)o + off, &v, 4); }
static unsigned long GetU(const void* o, unsigned off) { unsigned long v; memcpy(&v, (const unsigned char*)o + off, 4); return v; }

extern "C" void __fastcall FixInCtor(void* self, int, const void* source)
{
    Buf* b = Find(source); SetU(self, 4, 0); SetU(self, 8, b->len); SetU(self, 0xC, (unsigned long)b->data); SetU(self, 0x10, 0); SetU(self, 0x14, b->len);
}
extern "C" void __fastcall FixInDtor(void*) {}
extern "C" void __fastcall FixOutCtor(void* self) { Buf* b = Find(self); b->len = 0; }
extern "C" void __fastcall FixOutDtor(void* self) { Find(self)->owner = (const void*)1; }
extern "C" void __fastcall FixClear(void* self) { Buf* b = Find(self); b->len = 0; ++b->clears; }
extern "C" void __fastcall FixWrite(void* self, int, const void* d, long n) { Append(self, d, n); }
extern "C" void __fastcall FixWriteULONG(void* self, int, unsigned long v) { Append(self, &v, 4); }
extern "C" const void* __fastcall FixPeek(const void* self) { return Find(self)->data; }
extern "C" unsigned long __fastcall FixLength(const void* self) { return Find(self)->len; }
extern "C" void __fastcall FixCopyTo(void* self, int, void* dest, unsigned long n)
{
    const unsigned char* p = (const unsigned char*)GetU(self, 0xC); Append(dest, p, n);
    SetU(self, 0xC, (unsigned long)(p + n)); SetU(self, 4, GetU(self, 4) + n); SetU(self, 0x14, GetU(self, 0x14) - n);
}
extern "C" void __fastcall FixOverflow(void*, int, void*, long) { CHECK(!"no chunk overflow expected"); }
#pragma comment(linker, "/alternatename:??0CMemoryDataInputStream@@QAE@ABVCMemoryDataOutputStream@@@Z=@FixInCtor@12")
#pragma comment(linker, "/alternatename:??1CMemoryDataInputStream@@UAE@XZ=@FixInDtor@4")
#pragma comment(linker, "/alternatename:??0CMemoryDataOutputStream@@QAE@XZ=@FixOutCtor@4")
#pragma comment(linker, "/alternatename:??1CMemoryDataOutputStream@@UAE@XZ=@FixOutDtor@4")
#pragma comment(linker, "/alternatename:?Clear@CMemoryDataOutputStream@@QAEXXZ=@FixClear@4")
#pragma comment(linker, "/alternatename:?Write@CMemoryDataOutputStream@@UAEXPBXJ@Z=@FixWrite@16")
#pragma comment(linker, "/alternatename:?WriteULONG@CDataOutputStream@@QAEXK@Z=@FixWriteULONG@12")
#pragma comment(linker, "/alternatename:?PeekData@CMemoryDataOutputStream@@QBEPBXXZ=@FixPeek@4")
#pragma comment(linker, "/alternatename:?GetLength@CMemoryDataOutputStream@@QBEKXZ=@FixLength@4")
#pragma comment(linker, "/alternatename:?CopyTo@CDataInputStream@@QAEXAAVCDataOutputStream@@K@Z=@FixCopyTo@16")
#pragma comment(linker, "/alternatename:?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z=@FixOverflow@16")
extern "C" void __fastcall FixOptimise(void* self);
#pragma comment(linker, "/alternatename:@FixOptimise@4=?OptimiseConstraints@CParticleProgram@@AAEXXZ")

static unsigned char program[0x140];
static void* constraintsVtable[8];
static void Rec(unsigned long type, unsigned long count, unsigned long payloadBytes, unsigned char fill)
{
    const void* c = program + 0x114; unsigned long h[3] = { type, count, payloadBytes };
    Append(c, h, 12); unsigned char p[64]; memset(p, fill, payloadBytes); Append(c, p, payloadBytes);
}
static unsigned long W(const Buf* b, unsigned i) { return GetU(b->data, 4 * i); }

int main()
{
    for (int i = 0; i < 8; ++i) constraintsVtable[i] = (void*)FixWrite;
    void* vt = constraintsVtable; memcpy(program + 0x114, &vt, 4);
    // DIST(2,8,a) DIST(1,4,b) REPEAT(3) REPEAT(2) MINMAX(1,4,c) DIST(0,0) DIST(5,4,d) END(1) END(1)
    Rec(2, 2, 8, 0xA1); Rec(2, 1, 4, 0xB2); Rec(0, 3, 0, 0); Rec(0, 2, 0, 0); Rec(3, 1, 4, 0xC3); Rec(2, 0, 0, 0);
    Rec(2, 5, 4, 0xD4); Rec(1, 1, 0, 0); Rec(1, 1, 0, 0);   // AddConstraintEndRepeat writes (END_REPEAT, 1, 0)
    FixOptimise(program);
    const Buf* c = Find(program + 0x114);
    CHECK(c->clears == 1);
    // expected: DIST(3,12,a*8 b*4) REPEAT(3) REPEAT(2) MINMAX(1,4,c) DIST(5,4,d) END END
    // (DIST(0,0) opens a run with count 0 that DIST(5) then extends: count 5, size 4)
    unsigned long i = 0;
    CHECK(W(c, i) == 2 && W(c, i + 1) == 3 && W(c, i + 2) == 12); i += 3;
    for (int k = 0; k < 12; ++k) CHECK(c->data[4 * i + k] == (k < 8 ? 0xA1 : 0xB2));
    i += 3;
    CHECK(W(c, i) == 0 && W(c, i + 1) == 3 && W(c, i + 2) == 0); i += 3;
    CHECK(W(c, i) == 0 && W(c, i + 1) == 2 && W(c, i + 2) == 0); i += 3;
    CHECK(W(c, i) == 3 && W(c, i + 1) == 1 && W(c, i + 2) == 4); i += 3;
    CHECK(c->data[4 * i] == 0xC3); i += 1;
    CHECK(W(c, i) == 2 && W(c, i + 1) == 5 && W(c, i + 2) == 4); i += 3;
    CHECK(c->data[4 * i] == 0xD4); i += 1;
    CHECK(W(c, i) == 1 && W(c, i + 1) == 1 && W(c, i + 2) == 0); i += 3;
    CHECK(W(c, i) == 1 && W(c, i + 1) == 1 && W(c, i + 2) == 0); i += 3;
    CHECK(c->len == 4 * i);

    // A lone zero-count record is dropped; an empty program stays empty.
    Find(program + 0x114)->len = 0; Rec(2, 0, 0, 0);
    FixOptimise(program);
    CHECK(Find(program + 0x114)->len == 0);
    FixOptimise(program);
    CHECK(Find(program + 0x114)->len == 0);

    if (g_fail) { printf("OPTIMISE_CONSTRAINTS_FAIL %d\n", g_fail); return 1; }
    printf("OPTIMISE_CONSTRAINTS_OK\n");
    return 0;
}
