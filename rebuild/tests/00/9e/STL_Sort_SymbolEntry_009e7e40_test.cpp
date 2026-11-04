// Behaviour of _STL::sort<pair<CCharString, CSymbolInfo>*, CCompareString<CSymbolInfo> >
// 0x009E7E40: an empty range does nothing; otherwise __introsort_loop runs over
// the whole range with depth limit 2*floor(log2(n)), then the final insertion
// pass: one guarded pass over n <= 16 elements, or a guarded pass over the
// first 16 and an unguarded pass over the rest.
#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
typedef CCompareString<CSymbolMap::CSymbolInfo> Cmp;
static char g_log[256];
static int g_len;
static Entry* g_base;
static void Log(char tag, Entry* a, Entry* b, long extra)
{
    g_len += sprintf(g_log + g_len, "%c%d,%d,%ld;", tag, (int)(a - g_base), (int)(b - g_base), extra);
}
template<> void FABLE_FASTCALL _STL::__introsort_loop(Entry* first, Entry* last, Entry* tag, int depth, Cmp)
{
    Log('I', first, last, tag == 0 ? depth : -1);
}
template<> void FABLE_FASTCALL _STL::__insertion_sort(Entry* first, Entry* last, Cmp)
{
    Log('S', first, last, 0);
}
template<> void FABLE_FASTCALL _STL::__unguarded_insertion_sort_aux(Entry* first, Entry* last, Entry* tag, Cmp)
{
    Log('U', first, last, tag == 0 ? 0 : -1);
}

static int Check(int n, const char* want)
{
    g_len = 0; g_log[0] = 0;
    _STL::sort(g_base, g_base + n, Cmp());
    if (strcmp(g_log, want) != 0) { printf("n=%d got %s want %s\n", n, g_log, want); return 1; }
    return 0;
}

int main()
{
    g_base = reinterpret_cast<Entry*>(0x10000);
    if (Check(0, "") || Check(1, "I0,1,0;S0,1,0;") || Check(5, "I0,5,4;S0,5,0;") ||
        Check(16, "I0,16,8;S0,16,0;") || Check(17, "I0,17,8;S0,16,0;U16,17,0;") ||
        Check(40, "I0,40,10;S0,16,0;U16,40,0;"))
        return 1;
    puts("FABLETLC_STL_SORT_SYMBOLENTRY_009E7E40_PASS");
    return 0;
}