// Behaviour fixture for CParticleSimulation::WriteVectorArray (provider CParticleSimulation_WriteVectorArray_00ae18c0.cpp),
// reached through its mangled name (protected static __fastcall). The stream is a fake object whose vtable slot 4
// (Write) copies the block it is given; the temporary array's constructor is a stand-in. Checked for counts 0, 1, 3, 4, 5 and 9 (the unrolled loop's remainder paths):
// exactly one Write of count * 12 bytes holding each vector's x, y, z in order (w dropped); the source is not modified.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_particle_simulation.h"
#include "fable_data_output_stream.h"

static int g_fail, g_writes; static long g_size; static unsigned char g_copy[256];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static void __fastcall FakeWrite(void*, int, const void* data, long size)
{
    ++g_writes; g_size = size; if (size > 0 && size <= (long)sizeof(g_copy)) memcpy(g_copy, data, size);
}
// The temporary array's out-of-line constructor (retail 0x007C2AB0) lives elsewhere; a stand-in here (filled with -1).
namespace _STL
{
template <> vector<C3DVector, allocator<C3DVector> >::vector(size_t n)
    : _Vector_base<C3DVector, allocator<C3DVector> >(n, allocator<C3DVector>())
{
    _M_finish = _M_start + n;
    for (size_t i = 0; i < n; ++i) new (_M_start + i) C3DVector(-1.0f, -1.0f, -1.0f);
}
}
static void __fastcall NotReached(void*, int) { CHECK(!"other stream slot"); }

extern "C" void __fastcall FixWriteVectors(const void* vectors, unsigned long count, void* stream);
#pragma comment(linker, "/alternatename:@FixWriteVectors@12=?WriteVectorArray@CParticleSimulation@@KIXPBVC4DVector@@KAAVCDataOutputStream@@@Z")

int main()
{
    static void* vtable[8] = { (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)NotReached, (void*)FakeWrite,
                               (void*)NotReached, (void*)NotReached, (void*)NotReached };
    static void* stream[4] = { vtable };
    static float v[9][4], before[9][4];
    for (int i = 0; i < 9; ++i) for (int k = 0; k < 4; ++k) v[i][k] = (float)(i * 10 + k) + 0.5f;
    memcpy(before, v, sizeof(v));
    static const unsigned long counts[] = { 0, 1, 3, 4, 5, 9 };
    for (int c = 0; c < 6; ++c)
    {
        unsigned long n = counts[c];
        g_writes = 0; memset(g_copy, 0xEE, sizeof(g_copy));
        FixWriteVectors(v, n, stream);
        CHECK(g_writes == 1 && g_size == (long)(n * 12));
        for (unsigned long i = 0; i < n; ++i) CHECK(memcmp(g_copy + i * 12, v[i], 12) == 0);
        CHECK(memcmp(v, before, sizeof(v)) == 0);
    }
    if (g_fail) { printf("WRITE_VECTOR_ARRAY_FAIL %d\n", g_fail); return 1; }
    printf("WRITE_VECTOR_ARRAY_OK\n");
    return 0;
}
