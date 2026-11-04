// Behaviour fixture for _STL::vector<CParticleProgram (provider STLVectorLoopBegin_insert_overflow_00ae64c0.cpp). :CLoopBegin>::_M_insert_overflow (push_back past capacity): the vector reallocates (growing to at least double, or the fill length), keeps the old entries in order, appends fill_len copies of x at the end and updates size and capacity.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_particle_program.h"
#include "fable_stlport_vector_impl.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
typedef CParticleProgram::CLoopBegin Loop;
namespace _STL { template <> void vector<Loop>::_M_insert_overflow(Loop* pos, const Loop& x, const __false_type&, size_t n, bool atEnd); }
int main()
{
    {
        _STL::vector<Loop> v;
        for (unsigned long i = 0; i < 9; ++i) { Loop l; l.Position = i * 4; l.LoopCount = (long)i + 1; v.push_back(l); }
        CHECK(v.size() == 9 && v.capacity() >= 9);
        for (unsigned long i = 0; i < 9; ++i) CHECK(v[i].Position == i * 4 && v[i].LoopCount == (long)i + 1);
    }
    if (g_fail) { printf("LOOP_INSERT_OVERFLOW_FAIL %d\n", g_fail); return 1; }
    printf("LOOP_INSERT_OVERFLOW_OK\n");
    return 0;
}
