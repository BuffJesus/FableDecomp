// Behaviour of vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::reserve
// 0x009E72C0: a request within the capacity changes nothing; a larger one
// moves the entries (copy-construct, destroy the old ones, free the old block)
// into a block of exactly n elements; an empty vector just gets the block.
#include "fable_symbol_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fixture scaffolding: a string is its data pointer; count copies and frees.
static int g_copies, g_frees;
static long Raw(const CCharString& s) { return *reinterpret_cast<const long*>(&s); }
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<long*>(this) = Raw(other); }
CCharString::~CCharString() { ++g_frees; }

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
typedef _STL::vector<Entry> EntryVector;

int main()
{
    unsigned char store[sizeof(EntryVector)];
    memset(store, 0, sizeof(store));
    EntryVector* v = reinterpret_cast<EntryVector*>(store);
    v->reserve(0);
    if (v->_M_start != 0 || v->_M_end_of_storage._M_data != 0) { puts("zero"); return 1; }
    v->reserve(3);
    Entry* first = v->_M_start;
    if (first == 0 || v->_M_finish != first || v->_M_end_of_storage._M_data != first + 3 || g_copies != 0) { puts("empty"); return 1; }
    for (int i = 0; i < 2; ++i)
    {
        *reinterpret_cast<long*>(&first[i].first) = 10 + i;
        *reinterpret_cast<long*>(&first[i].second.GroupName) = 20 + i;
        *reinterpret_cast<long*>(&first[i].second.EntryName) = 30 + i;
        first[i].second.Val = 40 + i;
    }
    v->_M_finish = first + 2;
    v->reserve(3);
    v->reserve(2);
    if (v->_M_start != first || v->_M_finish != first + 2 || g_copies != 0) { puts("within"); return 1; }
    v->reserve(5);
    Entry* now = v->_M_start;
    if (now == first || v->_M_finish != now + 2 || v->_M_end_of_storage._M_data != now + 5) { puts("grow"); return 1; }
    for (int i = 0; i < 2; ++i)
        if (Raw(now[i].first) != 10 + i || Raw(now[i].second.GroupName) != 20 + i ||
            Raw(now[i].second.EntryName) != 30 + i || now[i].second.Val != 40 + i) { puts("copy"); return 1; }
    if (g_copies != 6 || g_frees != 6) { printf("copies %d frees %d\n", g_copies, g_frees); return 1; }
    puts("FABLETLC_VECTOR_SYMBOLENTRY_RESERVE_009E72C0_PASS");
    return 0;
}