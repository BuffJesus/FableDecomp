// Behaviour fixture for the nav CVectorMap Sort / Update instantiations (provider CVectorMap_NavSortUpdate_00a84590.cpp).
// Update must sort by the 64-bit key only while the map is dirty and then mark it clean; Sort must sort and mark it
// clean regardless of the flag. Keys straddle 2^32 so a 32-bit comparison would order them wrongly.
#include <stdio.h>
#include "fable_vector_map.h"

class CANavQuadTreeNode;
typedef CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<unsigned __int64, bool> > BoolMap;
typedef CVectorMap<unsigned __int64, CANavQuadTreeNode*, CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> > NodeMap;

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

// The STLport sorts are out of line per instantiation (retail 0x00A84160 / 0x00A841F0, not reconstructed here), so the
// fixture supplies them: an insertion sort through the map's comparator that counts its calls.
static int g_sorts;
template <class P, class C>
static void InsertionSort(P* first, P* last, C comp)
{
    ++g_sorts;
    for (P* i = first + 1; i < last; ++i)
        for (P* j = i; j > first && comp(*j, *(j - 1)); --j) { P t = *j; *j = *(j - 1); *(j - 1) = t; }
}
typedef _STL::pair<unsigned __int64, bool> BoolPair;
typedef _STL::pair<unsigned __int64, CANavQuadTreeNode*> NodePair;
namespace _STL
{
template <> void FABLE_FASTCALL sort<BoolPair*, CKeyPairCompareLess<unsigned __int64, bool> >(
    BoolPair* f, BoolPair* l, CKeyPairCompareLess<unsigned __int64, bool> c) { InsertionSort(f, l, c); }
template <> void FABLE_FASTCALL sort<NodePair*, CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> >(
    NodePair* f, NodePair* l, CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> c) { InsertionSort(f, l, c); }
}

// The provider's instantiations are called by mangled name (a member call here would instantiate the fixture's own
// inline copy): a no-argument __thiscall is call-compatible with a one-argument __fastcall (this in ecx).
extern "C" void __fastcall BoolSort(void*);
extern "C" void __fastcall BoolUpdate(void*);
extern "C" void __fastcall NodeSort(void*);
extern "C" void __fastcall NodeUpdate(void*);
#pragma comment(linker, "/alternatename:@BoolSort@4=?Sort@?$CVectorMap@_K_NV?$CKeyPairCompareLess@_K_N@@@@QAEXXZ")
#pragma comment(linker, "/alternatename:@BoolUpdate@4=?Update@?$CVectorMap@_K_NV?$CKeyPairCompareLess@_K_N@@@@QAEXXZ")
#pragma comment(linker, "/alternatename:@NodeSort@4=?Sort@?$CVectorMap@_KPAVCANavQuadTreeNode@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@@QAEXXZ")
#pragma comment(linker, "/alternatename:@NodeUpdate@4=?Update@?$CVectorMap@_KPAVCANavQuadTreeNode@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@@QAEXXZ")

// STLport push_back is out of line per instantiation, so the fixture points each map's vector at a local array
// (and detaches it before destruction).
template <class M, class P>
static void Attach(M& map, P* items, unsigned count)
{
    map._M_start = items; map._M_finish = items + count; map._M_end_of_storage._M_data = items + count;
    map.Dirty = true;
}
template <class M>
static void Detach(M& map) { map._M_start = map._M_finish = 0; map._M_end_of_storage._M_data = 0; }

template <class M>
static bool Sorted(M& map)
{
    for (unsigned i = 1; i < map.size(); ++i)
        if (map[i - 1].first > map[i].first) return false;
    return true;
}

int main()
{
    const unsigned __int64 big = (unsigned __int64)1 << 32;
    BoolMap flags;
    _STL::pair<unsigned __int64, bool> flagItems[4] = {
        _STL::pair<unsigned __int64, bool>(big + 5, true), _STL::pair<unsigned __int64, bool>(7, false),
        _STL::pair<unsigned __int64, bool>(big, true), _STL::pair<unsigned __int64, bool>(1, true) };
    Attach(flags, flagItems, 4);
    CHECK(flags.Dirty && !Sorted(flags));
    flags.Dirty = false;
    BoolUpdate(&flags);                          // clean: must not sort
    CHECK(!Sorted(flags) && !flags.Dirty && g_sorts == 0);
    flags.Dirty = true;
    BoolUpdate(&flags);
    CHECK(Sorted(flags) && !flags.Dirty && g_sorts == 1);
    CHECK(flags.size() == 4 && flags[0].first == 1 && flags[1].first == 7 && flags[2].first == big && flags[3].first == big + 5);
    CHECK(flags[0].second && !flags[1].second && flags[3].second);   // values travel with their keys
    BoolPair swap = flagItems[0]; flagItems[0] = flagItems[3]; flagItems[3] = swap;
    flags.Dirty = false;
    BoolSort(&flags);                        // sorts even when clean
    CHECK(Sorted(flags) && !flags.Dirty && g_sorts == 2);

    NodeMap nodes;
    CANavQuadTreeNode* a = reinterpret_cast<CANavQuadTreeNode*>(0x100);
    CANavQuadTreeNode* b = reinterpret_cast<CANavQuadTreeNode*>(0x200);
    _STL::pair<unsigned __int64, CANavQuadTreeNode*> nodeItems[3] = {
        _STL::pair<unsigned __int64, CANavQuadTreeNode*>(big * 3, a), _STL::pair<unsigned __int64, CANavQuadTreeNode*>(big * 2 + 9, b),
        _STL::pair<unsigned __int64, CANavQuadTreeNode*>(4, a) };
    Attach(nodes, nodeItems, 2);
    nodes.Dirty = false;
    NodeSort(&nodes);                            // sorts even when clean
    CHECK(Sorted(nodes) && !nodes.Dirty && nodes[0].second == b && nodes[1].second == a && g_sorts == 3);
    nodes._M_finish = nodeItems + 3; nodes._M_end_of_storage._M_data = nodeItems + 3; nodes.Dirty = true;   // as Insert
    NodeUpdate(&nodes);
    CHECK(Sorted(nodes) && nodes[0].first == 4 && !nodes.Dirty);

    Detach(flags); Detach(nodes);
    if (g_fail) { printf("NAV_VECTOR_MAP_SORT_FAIL %d\n", g_fail); return 1; }
    printf("NAV_VECTOR_MAP_SORT_OK\n");
    return 0;
}
