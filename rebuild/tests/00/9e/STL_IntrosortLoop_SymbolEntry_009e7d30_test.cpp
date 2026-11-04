// Behaviour of _STL::__introsort_loop<pair<CCharString, CSymbolInfo>*, ..., int,
// CCompareString<CSymbolInfo> > 0x009E7D30: runs of 16 or fewer are left alone;
// with the depth limit spent the range goes to __partial_sort(first, last, last);
// otherwise the median of first / middle / last-1 (by CCharString::FastCompare on
// the key) is copied as the pivot, __unguarded_partition splits the range, the
// right part recurses with depth-1 and the loop continues on the left part.
#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>

// Fixture scaffolding: a key is its data pointer, ordered numerically.
static long Key(const CCharString& s) { return *reinterpret_cast<const long*>(&s); }
bool CCharString::FastCompare(const CCharString& other) const { return Key(*this) < Key(other); }
CCharString::CCharString(const CCharString& other) { *reinterpret_cast<long*>(this) = Key(other); }
CCharString::~CCharString() {}

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
typedef CCompareString<CSymbolMap::CSymbolInfo> Cmp;
static double g_raw[64 * sizeof(Entry) / sizeof(double)];
#define g_a (reinterpret_cast<Entry*>(g_raw))
static char g_log[512];
static int g_len;
static int g_cuts[4];
static int g_cutIndex;
template<> Entry* FABLE_FASTCALL _STL::__unguarded_partition(Entry* first, Entry* last, Entry pivot, Cmp)
{
    g_len += sprintf(g_log + g_len, "P%d,%d,%ld,%ld;", (int)(first - g_a), (int)(last - g_a), Key(pivot.first),
                     pivot.second.Val);
    return g_a + g_cuts[g_cutIndex++];
}
template<> void FABLE_FASTCALL _STL::__partial_sort(Entry* first, Entry* middle, Entry* last, Entry* tag, Cmp)
{
    g_len += sprintf(g_log + g_len, "H%d,%d,%d,%d;", (int)(first - g_a), (int)(middle - g_a), (int)(last - g_a),
                     tag == 0);
}

static int Run(int first, int last, int depth, const char* want)
{
    g_len = 0; g_log[0] = 0; g_cutIndex = 0;
    _STL::__introsort_loop(g_a + first, g_a + last, (Entry*)0, depth, Cmp());
    if (strcmp(g_log, want) != 0) { printf("got %s want %s\n", g_log, want); return 1; }
    return 0;
}

static void Keys(int n, int a, int b, int c)
{
    // first, middle (first + n/2) and last-1 of [0, n)
    *reinterpret_cast<long*>(&g_a[0].first) = a; g_a[0].second.Val = 100;
    *reinterpret_cast<long*>(&g_a[n / 2].first) = b; g_a[n / 2].second.Val = 200;
    *reinterpret_cast<long*>(&g_a[n - 1].first) = c; g_a[n - 1].second.Val = 300;
}

int main()
{
    memset(g_raw, 0, sizeof(g_raw));
    if (Run(0, 16, 5, "")) return 1;                          // short run: untouched
    if (Run(0, 20, 0, "H0,20,20,1;")) return 1;               // depth spent: heap sort
    // depth 1: partition on the median, recurse right with depth 0, loop left (10 long).
    g_cuts[0] = 10;
    Keys(40, 5, 9, 7);
    if (Run(0, 40, 1, "P0,40,7,300;H10,40,40,1;")) return 1;
    Keys(40, 5, 3, 7);
    if (Run(0, 40, 1, "P0,40,5,100;H10,40,40,1;")) return 1;
    Keys(40, 5, 6, 7);
    if (Run(0, 40, 1, "P0,40,6,200;H10,40,40,1;")) return 1;
    Keys(40, 9, 3, 7);
    if (Run(0, 40, 1, "P0,40,7,300;H10,40,40,1;")) return 1;
    Keys(40, 9, 8, 7);
    if (Run(0, 40, 1, "P0,40,8,200;H10,40,40,1;")) return 1;
    // a cut near the end: the right part (6 long) needs no work and the left
    // part (34 long) loops with the depth one lower.
    g_cuts[0] = 34; g_cuts[1] = 10;
    Keys(40, 1, 2, 3);
    if (Run(0, 40, 2, "P0,40,2,200;P0,34,0,0;H10,34,34,1;")) return 1;
    puts("FABLETLC_STL_INTROSORT_LOOP_SYMBOLENTRY_009E7D30_PASS");
    return 0;
}