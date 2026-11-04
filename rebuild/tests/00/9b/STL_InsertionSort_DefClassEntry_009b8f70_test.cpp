// Behaviour of _STL::__insertion_sort<pair<unsigned long, CDefClassInfo>*,
// CKeyPairCompareLess<...> > 0x009B8F70: an empty range does nothing; otherwise
// every entry after the first is copied (key, name, allocator, its own copy of
// the index list) and handed by value to __linear_insert(first, i, copy, comp),
// in order.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fixture scaffolding: a string is its data pointer; count copies.
static int g_copies;
static unsigned long Raw(const CCharString& s) { return *reinterpret_cast<const unsigned long*>(&s); }
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<unsigned long*>(this) = Raw(other); }
CCharString::~CCharString() {}

typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long, Info> Entry;
typedef CKeyPairCompareLess<unsigned long, Info> Less;
static double g_raw[8 * sizeof(Entry) / sizeof(double)];
#define g_a (reinterpret_cast<Entry*>(g_raw))
static unsigned long g_index[8];
static char g_log[256];
static int g_len;
template<> void FABLE_FASTCALL _STL::__linear_insert(Entry* first, Entry* last, Entry val, Less)
{
    unsigned long idx = val.second.GlobalIndices._M_finish == val.second.GlobalIndices._M_start + 1 &&
                        val.second.GlobalIndices._M_start != &g_index[last - g_a]
                            ? *val.second.GlobalIndices._M_start : 0;
    g_len += sprintf(g_log + g_len, "L%d,%d,%lu,%lx,%lu;", (int)(first - g_a), (int)(last - g_a), val.first,
                     Raw(val.second.Name), idx);
}

int main()
{
    memset(g_raw, 0, sizeof(g_raw));
    for (int i = 0; i < 8; ++i)
    {
        g_a[i].first = 10 + i;
        *reinterpret_cast<unsigned long*>(&g_a[i].second.Name) = 0x20 + i;
        g_index[i] = 500 + i;
        g_a[i].second.GlobalIndices._M_start = &g_index[i];
        g_a[i].second.GlobalIndices._M_finish = &g_index[i] + 1;
        g_a[i].second.GlobalIndices._M_end_of_storage._M_data = &g_index[i] + 1;
    }
    _STL::__insertion_sort(g_a + 2, g_a + 2, Less());
    _STL::__insertion_sort(g_a + 2, g_a + 3, Less());
    if (g_len != 0 || g_copies != 0) { puts("short"); return 1; }
    _STL::__insertion_sort(g_a + 1, g_a + 4, Less());
    if (strcmp(g_log, "L1,2,12,22,502;L1,3,13,23,503;") != 0) { printf("log %s\n", g_log); return 1; }
    if (g_copies != 2) { printf("copies %d\n", g_copies); return 1; }
    puts("FABLETLC_STL_INSERTION_SORT_DEFCLASSENTRY_009B8F70_PASS");
    return 0;
}