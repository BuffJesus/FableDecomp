// Behaviour fixture for STLport's heap helpers and __partial_sort instantiated
// for pair<unsigned long, long> (the definition-name CRC/global-index map).
// Every helper is the real compiled provider; the fixture sees only the
// declarations. Entries carry a unique payload so a result can be checked as
// a permutation of its input.
#include "fable_vector_map.h"
#include "fable_stlport_heap.h"
#include <stdio.h>
#include <string.h>

typedef _STL::pair<unsigned long, long> Entry;
typedef CKeyPairCompareLess<unsigned long, long> Comp;

static unsigned long g_seed = 0x1d3f;
static unsigned long Next() { g_seed = g_seed * 1103515245u + 12345u; return g_seed >> 8; }
static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("INDEX_HEAP_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)

enum { kMax = 300 };
static Entry g_items[kMax], g_work[kMax];

static void Fill(int n, int shape)
{
    for (int i = 0; i < n; ++i)
    {
        unsigned long key = shape == 0 ? Next() % 4 : shape == 1 ? Next() : shape == 2 ? (unsigned long)i : (unsigned long)(n - i);
        if (shape == 4) key = (i & 1) ? 0xffffffffu : 0x80000000u;
        g_items[i].first = key;
        g_items[i].second = i;
    }
    memcpy(g_work, g_items, sizeof(Entry) * n);
}
static bool Permutation(int n)
{
    static unsigned char seen[kMax];
    memset(seen, 0, sizeof(seen));
    for (int i = 0; i < n; ++i)
    {
        long d = g_work[i].second;
        if (d < 0 || d >= n || seen[d] || g_work[i].first != g_items[d].first) return false;
        seen[d] = 1;
    }
    return true;
}
static bool Sorted(int to)
{
    for (int i = 1; i < to; ++i) if (g_work[i - 1].first > g_work[i].first) return false;
    return true;
}
static bool IsHeap(int n)
{
    for (int i = 1; i < n; ++i) if (g_work[(i - 1) / 2].first < g_work[i].first) return false;
    return true;
}
static unsigned long KthKey(int n, int k)
{
    for (int i = 0; i < n; ++i)
    {
        int below = 0, equal = 0;
        for (int j = 0; j < n; ++j) { below += g_items[j].first < g_items[i].first; equal += g_items[j].first == g_items[i].first; }
        if (below <= k && k < below + equal) return g_items[i].first;
    }
    return 0;
}

int main()
{
    static const int sizes[] = { 2, 3, 15, 16, 17, 33, 100, 300 };
    int cases = 0;
    for (int s = 0; s < 8; ++s) for (int shape = 0; shape < 5; ++shape)
    {
        const int n = sizes[s];
        Fill(n, shape);
        _STL::__make_heap(g_work, g_work + n, Comp(), (Entry*)0, (int*)0);
        CHECK(IsHeap(n) && Permutation(n), 1);
        _STL::sort_heap(g_work, g_work + n, Comp());
        CHECK(Sorted(n) && Permutation(n), 2);
        for (int m = 1; m <= n; m += (n + 2) / 3)
        {
            Fill(n, shape);
            _STL::__partial_sort(g_work, g_work + m, g_work + n, (Entry*)0, Comp());
            CHECK(Sorted(m) && Permutation(n) && g_work[m - 1].first == KthKey(n, m - 1), 3);
        }
        Fill(n, shape);
        _STL::__make_heap(g_work, g_work + n - 1, Comp(), (Entry*)0, (int*)0);
        _STL::__push_heap(g_work, n - 1, 0, g_work[n - 1], Comp());
        CHECK(IsHeap(n) && Permutation(n), 4);
        Entry root = g_work[0], last = g_work[n - 1];
        _STL::__adjust_heap(g_work, 0, n - 1, last, Comp());
        CHECK(IsHeap(n - 1), 5);
        g_work[n - 1] = root;
        CHECK(Permutation(n), 6);
        ++cases;
    }
    if (g_fail) return 1;
    printf("INDEX_HEAP_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
