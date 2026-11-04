// Behaviour of vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::_M_range_insert
// (pos, first, last, forward_iterator_tag) 0x009E73F0: an empty range does
// nothing; with room, the tail is shifted (constructing past the end,
// assigning backwards) and the range assigned or constructed in place, for both
// a long and a short tail; without room the entries before pos, the range and
// the entries after pos are copied into a block of size + max(size, n) and the
// old entries destroyed and freed.
#include "fable_symbol_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fixture scaffolding: a string is its data pointer; count copies and frees.
static int g_copies, g_frees, g_assigns;
static long Raw(const CCharString& s) { return *reinterpret_cast<const long*>(&s); }
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<long*>(this) = Raw(other); }
CCharString::~CCharString() { ++g_frees; }
CCharString& CCharString::operator=(const CCharString& other) { ++g_assigns; *reinterpret_cast<long*>(this) = Raw(other); return *this; }

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
typedef _STL::vector<Entry> EntryVector;

static void Put(Entry& e, long k)
{
    *reinterpret_cast<long*>(&e.first) = k;
    *reinterpret_cast<long*>(&e.second.GroupName) = k + 100;
    *reinterpret_cast<long*>(&e.second.EntryName) = k + 200;
    e.second.Val = k + 300;
}
static int Keys(const EntryVector& v, const char* want)
{
    char got[64]; int n = 0; got[0] = 0;
    for (const Entry* p = v._M_start; p != v._M_finish; ++p)
    {
        long k = Raw(p->first);
        if (Raw(p->second.GroupName) != k + 100 || Raw(p->second.EntryName) != k + 200 || p->second.Val != k + 300)
            return 1;
        n += sprintf(got + n, "%ld,", k);
    }
    if (strcmp(got, want) != 0) { printf("got %s want %s\n", got, want); return 1; }
    return 0;
}
static EntryVector* Make(unsigned char* store, int size, int cap)
{
    EntryVector* v = reinterpret_cast<EntryVector*>(store);
    Entry* b = (Entry*)malloc(cap * sizeof(Entry));
    for (int i = 0; i < size; ++i) Put(b[i], i + 1);
    v->_M_start = b; v->_M_finish = b + size; v->_M_end_of_storage._M_data = b + cap;
    return v;
}

int main()
{
    double srcRaw[3 * sizeof(Entry) / sizeof(double)];
    Entry* src = reinterpret_cast<Entry*>(srcRaw);
    for (int i = 0; i < 3; ++i) Put(src[i], 7 + i);   // 7, 8, 9
    unsigned char s1[sizeof(EntryVector)], s2[sizeof(EntryVector)], s3[sizeof(EntryVector)];
    _STL::forward_iterator_tag tag;

    // Room, long tail (4 after pos > 2 inserted): 1,[7,8],2,3,4,5
    EntryVector* v = Make(s1, 5, 8);
    Entry* b = v->_M_start;
    v->_M_range_insert(b + 1, src, src + 2, tag);
    if (v->_M_start != b || Keys(*v, "1,7,8,2,3,4,5,")) { puts("long tail"); return 1; }
    v->_M_range_insert(b + 2, src, src, tag);
    if (Keys(*v, "1,7,8,2,3,4,5,")) { puts("empty"); return 1; }

    // Room, short tail (1 after pos <= 3 inserted): 1,2,[7,8,9],3
    v = Make(s2, 3, 8);
    b = v->_M_start;
    v->_M_range_insert(b + 2, src, src + 3, tag);
    if (v->_M_start != b || Keys(*v, "1,2,7,8,9,3,")) { puts("short tail"); return 1; }

    // No room: 3 + 2 > 4, new block of 3 + max(3, 2) = 6
    v = Make(s3, 3, 4);
    b = v->_M_start;
    g_copies = g_frees = 0;
    v->_M_range_insert(b + 1, src, src + 2, tag);
    if (v->_M_start == b || v->_M_end_of_storage._M_data != v->_M_start + 6 || Keys(*v, "1,7,8,2,3,")) { puts("grow"); return 1; }
    if (g_copies != 15 || g_frees != 9) { printf("copies %d frees %d\n", g_copies, g_frees); return 1; }
    puts("FABLETLC_VECTOR_SYMBOLENTRY_RANGE_INSERT_009E73F0_PASS");
    return 0;
}