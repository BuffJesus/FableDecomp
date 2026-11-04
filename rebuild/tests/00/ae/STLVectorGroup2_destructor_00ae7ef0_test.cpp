// Behaviour fixture for _STL::vector<C3DGroup2> (provider STLVectorGroup2_destructor_00ae7ef0.cpp). :~vector: every group (and its blend list) is destroyed and the storage freed; an empty vector is fine.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_particle_program.h"
#include "fable_stlport_vector_impl.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
C3DGroup2::C3DGroup2() : BoneIndex(-1) {}
void C3DGroup2::SetBoneIndex(long index) { BoneIndex = index; }
void C3DGroup2::AddVertexBlend(const C3DVertexBlend2& blend) { VertexBlends.push_back(blend); }
static int g_groupDtors;
namespace _STL { template <> vector<C3DGroup2>::~vector(); }
struct Counted { _STL::vector<C3DGroup2> v; };
int main()
{
    {
        Counted* c = new Counted;
        for (long i = 0; i < 3; ++i) { C3DGroup2 g; g.SetBoneIndex(i); g.AddVertexBlend(C3DVertexBlend2(i, 1.0f)); c->v.push_back(g); }
        delete c;
        Counted* e = new Counted;
        delete e;
        CHECK(1);
    }
    if (g_fail) { printf("GROUP2_VECTOR_DTOR_FAIL %d\n", g_fail); return 1; }
    printf("GROUP2_VECTOR_DTOR_OK\n");
    return 0;
}
