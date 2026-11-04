// Behaviour of vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::_M_insert_overflow
// 0x009E77E0: the storage grows to size + max(size, fill) elements, the entries
// before the position are copied, then `fill` copies of x, then (unless at end)
// the entries from the position on; the old entries are destroyed and the old
// block freed; start/finish/end-of-storage then describe the new block.
#include "fable_symbol_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fixture scaffolding: copies share the source's data pointer.
static int g_copies;
static int g_dtors;
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<void**>(this) = *reinterpret_cast<void* const*>(&other); }
CCharString::~CCharString() { ++g_dtors; }

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
typedef _STL::vector<Entry> EntryVector;
static void* Raw(const CCharString& s) { return *reinterpret_cast<void* const*>(&s); }
static void Set(CCharString& s, unsigned long v) { *reinterpret_cast<unsigned long*>(&s) = v; }
static void Fill(Entry& e, unsigned long k)
{
    Set(e.first, 0x100 + k);
    Set(e.second.GroupName, 0x200 + k);
    Set(e.second.EntryName, 0x300 + k);
    e.second.Val = (int)(10 + k);
}
static bool Is(const Entry& e, unsigned long k)
{
    return Raw(e.first) == (void*)(0x100 + k) && Raw(e.second.GroupName) == (void*)(0x200 + k) &&
           Raw(e.second.EntryName) == (void*)(0x300 + k) && e.second.Val == (int)(10 + k);
}

int main()
{
    unsigned char xs[sizeof(Entry)];
    Entry* x = reinterpret_cast<Entry*>(xs);
    Fill(*x, 9);
    unsigned char vstore[sizeof(EntryVector)];
    EntryVector* v = reinterpret_cast<EntryVector*>(vstore);
    _STL::__false_type tag;

    // Middle insert of one entry into a full two-entry vector: capacity 4,
    // order 0, x, 1; both old entries destroyed.
    Entry* old = (Entry*)malloc(2 * sizeof(Entry));
    Fill(old[0], 0);
    Fill(old[1], 1);
    v->_M_start = old;
    v->_M_finish = old + 2;
    v->_M_end_of_storage._M_data = old + 2;
    v->_M_insert_overflow(old + 1, *x, tag, 1, false);
    Entry* n = v->_M_start;
    if (n == old || v->_M_finish != n + 3 || v->_M_end_of_storage._M_data != n + 4) { puts("grow"); return 1; }
    if (!Is(n[0], 0) || !Is(n[1], 9) || !Is(n[2], 1)) { puts("order"); return 1; }
    if (g_copies != 9 || g_dtors != 6) { puts("counts"); return 1; }

    // At-end insert of three copies: capacity 3 + max(3, 3) = 6, the tail
    // copy is skipped, the three old entries destroyed.
    g_copies = g_dtors = 0;
    v->_M_insert_overflow(v->_M_finish, *x, tag, 3, true);
    Entry* m = v->_M_start;
    if (m == n || v->_M_finish != m + 6 || v->_M_end_of_storage._M_data != m + 6) { puts("fill"); return 1; }
    if (!Is(m[0], 0) || !Is(m[1], 9) || !Is(m[2], 1) || !Is(m[3], 9) || !Is(m[4], 9) || !Is(m[5], 9)) { puts("fillorder"); return 1; }
    if (g_copies != 18 || g_dtors != 9) { puts("fillcounts"); return 1; }

    // Empty vector: one element allocated, nothing destroyed or freed.
    g_copies = g_dtors = 0;
    v->_M_start = v->_M_finish = v->_M_end_of_storage._M_data = 0;
    v->_M_insert_overflow(0, *x, tag, 1, true);
    if (v->_M_start == 0 || v->_M_finish != v->_M_start + 1 || v->_M_end_of_storage._M_data != v->_M_start + 1 ||
        !Is(v->_M_start[0], 9) || g_copies != 3 || g_dtors != 0) { puts("empty"); return 1; }
    puts("FABLETLC_VECTOR_SYMBOLENTRY_INSERT_OVERFLOW_009E77E0_PASS");
    return 0;
}