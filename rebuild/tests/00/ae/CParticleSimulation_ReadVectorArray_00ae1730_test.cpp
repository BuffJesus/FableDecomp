// Behaviour fixture for CParticleSimulation::ReadVectorArray (provider CParticleSimulation_ReadVectorArray_00ae1730.cpp).
// The stream is raw storage over an in-memory buffer (StreamPos +0x04, CurrentSourcePtr +0x0C, bytes left +0x14); the
// fixture supplies the out-of-line CArray<C3DVector>(count) constructor and the chunk-overflow reader (recording).
// Checked for counts 0..9 (both the four-at-a-time and the remainder copies): the temporary holds count vectors read in
// one block, each destination vector gets the stored X/Y/Z and the given W, nothing past count is written, the stream
// advances by exactly 12 bytes per vector, and the temporary is released.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_particle_simulation.h"
#include "fable_data_input_stream.h"
#include "fable_carray.h"
#include "fable_2d_vector.h"
#include "fable_4d_vector.h"

static int g_fail, g_overflows, g_arrays;
static unsigned long g_lastCount;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CDataInputStream::ReadWithSrcChunkOverflow(void*, long) { ++g_overflows; }
namespace _STL
{
template <> vector<C3DVector, allocator<C3DVector> >::vector(size_t n)
    : _Vector_base<C3DVector, allocator<C3DVector> >(n, allocator<C3DVector>())
{
    ++g_arrays; g_lastCount = (unsigned long)n;
    _M_finish = _M_start + n;
    for (size_t i = 0; i < n; ++i) new (_M_start + i) C3DVector(-1.0f, -1.0f, -1.0f);
}
}

// Protected static: reach it by its mangled name.
extern "C" void __fastcall ReadVectors(C4DVector* vectors, unsigned long count, CDataInputStream* stream, float w);
#pragma comment(linker, "/alternatename:@ReadVectors@16=?ReadVectorArray@CParticleSimulation@@KIXPAVC4DVector@@KAAVCDataInputStream@@M@Z")

static unsigned char g_stream[0x18];
static void Attach(const void* data, long size)
{
    memset(g_stream, 0, sizeof(g_stream));
    const unsigned char* src = (const unsigned char*)data;
    memcpy(g_stream + 0x0C, &src, 4); memcpy(g_stream + 0x14, &size, 4);
}

int main()
{
    static float data[3 * 12];
    for (int i = 0; i < 3 * 12; ++i) data[i] = (float)(i + 1);
    for (unsigned long count = 0; count <= 9; ++count)
    {
        C4DVector out[12];
        memset(out, 0x5A, sizeof(out));
        Attach(data, sizeof(data));
        int arraysBefore = g_arrays;
        ReadVectors(out, count, reinterpret_cast<CDataInputStream*>(g_stream), 2.0f);
        CHECK(g_arrays == arraysBefore + 1 && g_lastCount == count);
        for (unsigned long i = 0; i < count; ++i)
            CHECK(out[i].X == data[3 * i] && out[i].Y == data[3 * i + 1] && out[i].Z == data[3 * i + 2] && out[i].W == 2.0f);
        unsigned char guard[sizeof(C4DVector)]; memset(guard, 0x5A, sizeof(guard));
        CHECK(memcmp(&out[count], guard, sizeof(guard)) == 0);
        unsigned long pos; memcpy(&pos, g_stream + 4, 4); CHECK(pos == 12 * count);
    }
    CHECK(g_overflows == 0);

    if (g_fail) { printf("VECTOR_ARRAY_FAIL %d\n", g_fail); return 1; }
    printf("VECTOR_ARRAY_OK\n");
    return 0;
}
