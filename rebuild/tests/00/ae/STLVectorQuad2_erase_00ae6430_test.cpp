// Behaviour fixture for _STL::vector<C3DQuad2> (provider STLVectorQuad2_erase_00ae6430.cpp). :erase(first, last): later quads move down element by element, the size shrinks and the first position is returned.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_particle_program.h"
#include "fable_stlport_vector_impl.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
namespace _STL { template <> C3DQuad2* vector<C3DQuad2>::erase(C3DQuad2* first, C3DQuad2* last); }
int main()
{
    {
        _STL::vector<C3DQuad2> v;
        for (unsigned short i = 0; i < 4; ++i) { C3DQuad2 q; for (int k = 0; k < 4; ++k) q.VertexIndex[k] = (unsigned short)(i * 10 + k); v.push_back(q); }
        C3DQuad2* r = v.erase(v.begin(), v.begin() + 2);
        CHECK(r == v.begin() && v.size() == 2 && v[0].VertexIndex[0] == 20 && v[1].VertexIndex[3] == 33);
        r = v.erase(v.begin() + 1, v.end());
        CHECK(r == v.begin() + 1 && v.size() == 1 && v[0].VertexIndex[2] == 22);
    }
    if (g_fail) { printf("QUAD_ERASE_FAIL %d\n", g_fail); return 1; }
    printf("QUAD_ERASE_OK\n");
    return 0;
}
