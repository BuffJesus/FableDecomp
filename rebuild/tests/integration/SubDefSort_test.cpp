// Behaviour fixture for STLport's sort, heap and insertion helpers and
// CVectorMap::LowerBound (both overloads) instantiated for
// pair<unsigned long, CDefClassBase::CSubDefInfo> (the parent def's sub-def
// table). Every helper is the real compiled provider; the fixture sees only
// the declarations, so each call links against them. Entries carry a unique
// DefIndex so a result can be checked as a permutation of its input.
#include "fable_def_class_base.h"
#include "fable_stlport_heap.h"
#include <stdio.h>
#include <string.h>

typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> Entry;
typedef CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> Comp;
typedef CParentDefClassBase::CSubDefInfoMap Map;

static unsigned long g_seed = 0x5d3f;
static unsigned long Next() { g_seed = g_seed * 1103515245u + 12345u; return g_seed >> 8; }
static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("SUBDEF_SORT_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)

enum { kMax = 300 };
static Entry g_items[kMax], g_work[kMax];

static void Fill(int n, int shape)
{
    for (int i = 0; i < n; ++i)
    {
        unsigned long key = shape == 0 ? Next() % 4 : shape == 1 ? Next() : shape == 2 ? (unsigned long)i : (unsigned long)(n - i);
        if (shape == 4) key = (i & 1) ? 0xffffffffu : 0x80000000u;
        g_items[i].first = key;
        g_items[i].second.DefIndex = i;
        g_items[i].second.OriginalParentDefIndex = ~i;
    }
    memcpy(g_work, g_items, sizeof(Entry) * n);
}

// Same multiset of (key, payload) as the input, with each payload intact.
static bool Permutation(int n)
{
    static unsigned char seen[kMax];
    memset(seen, 0, sizeof(seen));
    for (int i = 0; i < n; ++i)
    {
        long d = g_work[i].second.DefIndex;
        if (d < 0 || d >= n || seen[d] || g_work[i].second.OriginalParentDefIndex != ~d || g_work[i].first != g_items[d].first) return false;
        seen[d] = 1;
    }
    return true;
}
static bool Sorted(int from, int to)
{
    for (int i = from + 1; i < to; ++i) if (g_work[i - 1].first > g_work[i].first) return false;
    return true;
}
static bool IsHeap(int n)
{
    for (int i = 1; i < n; ++i) if (g_work[(i - 1) / 2].first < g_work[i].first) return false;
    return true;
}
static unsigned long KthKey(int n, int k)
{
    // The k-th smallest key of the input (counting sort on the side).
    int below;
    for (int i = 0; i < n; ++i)
    {
        below = 0; int equal = 0;
        for (int j = 0; j < n; ++j) { below += g_items[j].first < g_items[i].first; equal += g_items[j].first == g_items[i].first; }
        if (below <= k && k < below + equal) return g_items[i].first;
    }
    return 0;
}

int main()
{
    static const int sizes[] = { 0, 1, 2, 3, 15, 16, 17, 33, 100, 300 };
    int cases = 0;
    for (int s = 0; s < 10; ++s) for (int shape = 0; shape < 5; ++shape)
    {
        const int n = sizes[s];
        Fill(n, shape);
        _STL::sort(g_work, g_work + n, Comp());
        CHECK(Sorted(0, n) && Permutation(n), 1);
        Fill(n, shape);
        _STL::__introsort_loop(g_work, g_work + n, (Entry*)0, 0, Comp());  // depth exhausted: heap sort
        CHECK(Permutation(n) && (n <= 16 || Sorted(0, n)), 2);
        Fill(n, shape);
        _STL::__insertion_sort(g_work, g_work + n, Comp());
        CHECK(Sorted(0, n) && Permutation(n), 3);
        if (n >= 2)
        {
            Fill(n, shape);
            _STL::__make_heap(g_work, g_work + n, Comp(), (Entry*)0, (int*)0);
            CHECK(IsHeap(n) && Permutation(n), 4);
            _STL::sort_heap(g_work, g_work + n, Comp());
            CHECK(Sorted(0, n) && Permutation(n), 5);
            const int middle = n / 2 + 1;
            Fill(n, shape);
            _STL::__partial_sort(g_work, g_work + middle, g_work + n, (Entry*)0, Comp());
            CHECK(Sorted(0, middle) && Permutation(n) && g_work[middle - 1].first == KthKey(n, middle - 1), 6);
            Fill(n, shape);
            const Entry pivot = g_items[n / 2];
            Entry* cut = _STL::__unguarded_partition(g_work, g_work + n, pivot, Comp());
            CHECK(cut >= g_work && cut <= g_work + n && Permutation(n), 7);
            for (Entry* p = g_work; p < cut; ++p) CHECK(p->first <= pivot.first, 8);
            for (Entry* p = cut; p < g_work + n; ++p) CHECK(p->first >= pivot.first, 9);
            // Unguarded insertion: the minimum sits before the range.
            Fill(n, shape);
            int low = 0;
            for (int i = 1; i < n; ++i) if (g_work[i].first < g_work[low].first) low = i;
            Entry t = g_work[0]; g_work[0] = g_work[low]; g_work[low] = t;
            _STL::__unguarded_insertion_sort_aux(g_work + 1, g_work + n, (Entry*)0, Comp());
            CHECK(Sorted(0, n) && Permutation(n), 10);
            // Heap push and adjust.
            Fill(n, shape);
            _STL::__make_heap(g_work, g_work + n - 1, Comp(), (Entry*)0, (int*)0);
            _STL::__push_heap(g_work, n - 1, 0, g_work[n - 1], Comp());
            CHECK(IsHeap(n) && Permutation(n), 11);
            Entry root = g_work[0], last = g_work[n - 1];
            _STL::__adjust_heap(g_work, 0, n - 1, last, Comp());
            CHECK(IsHeap(n - 1), 12);
            g_work[n - 1] = root;
            CHECK(Permutation(n), 13);
        }
        // LowerBound over the sorted, de-duplicated keys (both overloads).
        Fill(n, shape);
        _STL::sort(g_work, g_work + n, Comp());
        int unique = 0;
        for (int i = 0; i < n; ++i) if (!unique || g_work[unique - 1].first != g_work[i].first) g_work[unique++] = g_work[i];
        static union { double align; unsigned char bytes[sizeof(Map)]; } storage;
        memset(&storage, 0, sizeof(storage));
        Map* map = reinterpret_cast<Map*>(&storage);
        map->_M_start = g_work; map->_M_finish = g_work + unique; map->_M_end_of_storage._M_data = g_work + unique;
        static const unsigned long probes[] = { 0, 1, 2, 3, 50, 0x7fffffff, 0x80000000, 0xfffffffe, 0xffffffff };
        for (int p = 0; p < 9 + unique; ++p)
        {
            const unsigned long key = p < 9 ? probes[p] : g_work[p - 9].first;
            int expect = 0;
            while (expect < unique && g_work[expect].first < key) ++expect;
            CHECK(map->LowerBound(key) == g_work + expect, 14);
            CHECK(static_cast<const Map*>(map)->LowerBound(key) == g_work + expect, 15);
        }
        ++cases;
    }
    if (g_fail) return 1;
    printf("SUBDEF_SORT_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
