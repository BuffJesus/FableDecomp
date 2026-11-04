// Behaviour fixture for _STL::vector<pair<CCharString, unsigned long> > (provider STLVectorExportParticle_erase_00ae6a20.cpp). :erase(first, last): later exports are assigned down (name and particle), each vacated tail name is destroyed, the size shrinks and the first position is returned. CCharString assignment and destruction are supplied here (a name is a 4-byte pointer).
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_particle_program.h"
#include "fable_stlport_vector_impl.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
static int g_assigns, g_dtors;
CCharString& CCharString::operator=(const CCharString& o) { ++g_assigns; memcpy(this, &o, 4); return *this; }
CCharString::~CCharString() { ++g_dtors; }
typedef _STL::pair<CCharString, unsigned long> Export;
namespace _STL { template <> Export* vector<Export>::erase(Export* first, Export* last); }
int main()
{
    static unsigned long raw[4][2] = { { 0x10, 1 }, { 0x20, 2 }, { 0x30, 3 }, { 0x40, 4 } };
    unsigned long v[3]; unsigned long* b = &raw[0][0]; unsigned long* e = b + 8; memcpy(v, &b, 4); memcpy(v + 1, &e, 4); memcpy(v + 2, &e, 4);
    _STL::vector<Export>& x = *reinterpret_cast<_STL::vector<Export>*>(v);
    Export* r = x.erase(x.begin() + 1, x.begin() + 2);
    CHECK(r == x.begin() + 1 && x.size() == 3 && g_assigns == 2 && g_dtors == 1);
    CHECK(raw[1][0] == 0x30 && raw[1][1] == 3 && raw[2][0] == 0x40 && raw[2][1] == 4 && raw[0][0] == 0x10);
    if (g_fail) { printf("EXPORT_ERASE_FAIL %d\n", g_fail); return 1; }
    printf("EXPORT_ERASE_OK\n");
    return 0;
}
