// Behaviour of _STL::__insertion_sort<pair<CCharString, CSymbolInfo>*,
// CCompareString<CSymbolInfo> > 0x009E7590: an empty range does nothing;
// otherwise every element after the first is copied (key, group, entry name,
// value) and handed by value to __linear_insert(first, i, copy, comp), in order.
#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>

// Fixture scaffolding: a string is its data pointer; count copies and frees.
static int g_copies, g_frees;
static long Raw(const CCharString& s) { return *reinterpret_cast<const long*>(&s); }
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<long*>(this) = Raw(other); }
CCharString::~CCharString() { ++g_frees; }

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
typedef CCompareString<CSymbolMap::CSymbolInfo> Cmp;
static double g_raw[8 * sizeof(Entry) / sizeof(double)];
#define g_a (reinterpret_cast<Entry*>(g_raw))
static char g_log[256];
static int g_len;
template<> void FABLE_FASTCALL _STL::__linear_insert(Entry* first, Entry* last, Entry val, Cmp)
{
    g_len += sprintf(g_log + g_len, "L%d,%d,%ld,%ld,%ld,%ld;", (int)(first - g_a), (int)(last - g_a), Raw(val.first),
                     Raw(val.second.GroupName), Raw(val.second.EntryName), val.second.Val);
}

int main()
{
    for (int i = 0; i < 8; ++i)
    {
        *reinterpret_cast<long*>(&g_a[i].first) = 10 + i;
        *reinterpret_cast<long*>(&g_a[i].second.GroupName) = 20 + i;
        *reinterpret_cast<long*>(&g_a[i].second.EntryName) = 30 + i;
        g_a[i].second.Val = 40 + i;
    }
    _STL::__insertion_sort(g_a + 2, g_a + 2, Cmp());
    _STL::__insertion_sort(g_a + 2, g_a + 3, Cmp());
    if (g_len != 0 || g_copies != 0) { puts("short"); return 1; }
    _STL::__insertion_sort(g_a + 1, g_a + 5, Cmp());
    if (strcmp(g_log, "L1,2,12,22,32,42;L1,3,13,23,33,43;L1,4,14,24,34,44;") != 0) { printf("log %s\n", g_log); return 1; }
    if (g_copies != 9 || g_frees != 9) { printf("copies %d frees %d\n", g_copies, g_frees); return 1; }
    puts("FABLETLC_STL_INSERTION_SORT_SYMBOLENTRY_009E7590_PASS");
    return 0;
}