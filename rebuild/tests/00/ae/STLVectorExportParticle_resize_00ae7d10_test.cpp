// Behaviour fixture for _STL::vector<pair<CCharString, unsigned long> >::resize(size_t) (provider
// STLVectorExportParticle_resize_00ae7d10.cpp). CCharString is supplied here as a counted 4-byte name pointer. Checked:
// growing appends default entries (empty name, index 0) after the kept ones; shrinking destroys exactly the removed
// names (plus the temporary default entry) and keeps the leading entries without reassigning them.
#define FABLE_VECTOR_RESIZE_OUT_OF_LINE
#include <stdio.h>
#include <new>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail, g_defaults, g_copies, g_destroys, g_assigns;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CCharString::CCharString() { ++g_defaults; const char* none = 0; memcpy(this, &none, 4); }
CCharString::CCharString(const CCharString& other) { ++g_copies; memcpy(this, &other, 4); }
CCharString::~CCharString() { ++g_destroys; }
CCharString& CCharString::operator=(const CCharString& other) { ++g_assigns; memcpy(this, &other, 4); return *this; }

typedef _STL::pair<CCharString, unsigned long> Export;
// STLport's out-of-line uninitialized_fill_n is another unit's (retail instantiation): supply a plain copy loop.
namespace _STL
{
template <> Export* FABLE_FASTCALL uninitialized_fill_n<Export>(Export* first, size_t n, const Export& x)
{
    for (size_t i = 0; i < n; ++i) new (first + i) Export(x);
    return first + n;
}
}

static const char* Name(const Export& e) { const char* n; memcpy(&n, &e.first, 4); return n; }

int main()
{
    {
        _STL::vector<Export> v;
        v.resize(2);
        CHECK(v.size() == 2 && !Name(v[0]) && v[1].second == 0);
        const char* tip = "tip";
        memcpy(&v[1].first, &tip, 4);
        v[1].second = 9;
        v.resize(5);
        CHECK(v.size() == 5 && Name(v[1]) == tip && v[1].second == 9 && !Name(v[4]) && v[4].second == 0);
        int before = g_destroys;
        v.resize(2);
        CHECK(v.size() == 2 && Name(v[1]) == tip && v[1].second == 9);
        CHECK(g_destroys - before == 3 + 1);   // three removed names, plus the temporary default entry's name
        CHECK(g_assigns == 0);
    }
    if (g_fail) { printf("EXPORT_RESIZE_FAIL %d\n", g_fail); return 1; }
    printf("EXPORT_RESIZE_OK\n");
    return 0;
}
