// Behaviour fixture for CParticleProgram::WriteGroups (provider CParticleProgram_WriteGroups_00ae0680.cpp), reached
// through its mangled name (private static __fastcall). The stream is a fake object whose vtable slot 4 (Write) records
// raw blocks; WriteULONG (out of line, retail 0x00993EA0) and C3DGroup2::PeekVertexBlends are supplied here. Groups are
// raw C3DGroup2 storage (BoneIndex +0, blend vector +4/+8/+12). Checked: the output is the group count, then for each
// group in order its bone index, its blend count and one raw block of count * 8 blend bytes taken from that group's
// blends (a group without blends still writes its index and a zero count, then an empty block); an empty list writes
// only the zero count.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"
#include "fable_data_output_stream.h"

static int g_fail, g_n;
struct Op { int kind; unsigned long value; const void* data; long size; } g_ops[32];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CDataOutputStream::WriteULONG(unsigned long value) { Op o = { 1, value, 0, 0 }; if (g_n < 32) g_ops[g_n] = o; ++g_n; }
const C3DVertexBlend2* C3DGroup2::PeekVertexBlends() const
{
    const C3DVertexBlend2* b; memcpy(&b, reinterpret_cast<const unsigned char*>(this) + 4, 4); return b;
}
static void __fastcall FakeWrite(void*, int, const void* data, long size) { Op o = { 2, 0, data, size }; if (g_n < 32) g_ops[g_n] = o; ++g_n; }
static void __fastcall NotReached(void*, int) { CHECK(!"other stream slot"); }

extern "C" void __fastcall FixWriteGroups(void* stream, const void* groups);
#pragma comment(linker, "/alternatename:@FixWriteGroups@8=?WriteGroups@CParticleProgram@@CIXAAVCDataOutputStream@@ABV?$CArray@VC3DGroup2@@@@@Z")

static void SetGroup(unsigned long* g, long bone, void* b, unsigned n)
{
    memcpy(g, &bone, 4); unsigned char* s = (unsigned char*)b; unsigned char* e = s + n * 8;
    memcpy(g + 1, &s, 4); memcpy(g + 2, &e, 4); memcpy(g + 3, &e, 4);
}

int main()
{
    static void* vtable[8] = { (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)FakeWrite,
                               (void*)NotReached, (void*)NotReached, (void*)NotReached };
    static void* stream[4] = { vtable };
    static unsigned long blendsA[3][2], blendsC[1][2];
    static unsigned long groups[3][4];
    SetGroup(groups[0], 7, blendsA, 3); SetGroup(groups[1], -1, 0, 0); SetGroup(groups[2], 2, blendsC, 1);
    unsigned long list[3]; void* gs = groups; void* ge = groups + 3; memcpy(list, &gs, 4); memcpy(list + 1, &ge, 4); memcpy(list + 2, &ge, 4);

    FixWriteGroups(stream, list);
    CHECK(g_n == 10);
    CHECK(g_ops[0].kind == 1 && g_ops[0].value == 3);
    CHECK(g_ops[1].kind == 1 && g_ops[1].value == 7 && g_ops[2].kind == 1 && g_ops[2].value == 3);
    CHECK(g_ops[3].kind == 2 && g_ops[3].data == blendsA && g_ops[3].size == 24);
    CHECK(g_ops[4].kind == 1 && g_ops[4].value == 0xFFFFFFFFu && g_ops[5].kind == 1 && g_ops[5].value == 0);
    CHECK(g_ops[6].kind == 2 && g_ops[6].data == 0 && g_ops[6].size == 0);
    CHECK(g_ops[7].kind == 1 && g_ops[7].value == 2 && g_ops[8].kind == 1 && g_ops[8].value == 1);
    CHECK(g_ops[9].kind == 2 && g_ops[9].data == blendsC && g_ops[9].size == 8);

    g_n = 0; memcpy(list + 1, &gs, 4);
    FixWriteGroups(stream, list);
    CHECK(g_n == 1 && g_ops[0].kind == 1 && g_ops[0].value == 0);

    if (g_fail) { printf("WRITE_GROUPS_FAIL %d\n", g_fail); return 1; }
    printf("WRITE_GROUPS_OK\n");
    return 0;
}
