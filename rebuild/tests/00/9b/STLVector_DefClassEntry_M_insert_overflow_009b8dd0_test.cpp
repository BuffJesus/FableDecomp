// Behaviour of vector<pair<unsigned long, CDefinitionManager::CDefClassInfo> >::
// _M_insert_overflow 0x009B8DD0: the storage grows to size + max(size, fill)
// elements, the entries before the position are copied (key, name, allocator,
// an own copy of the index list), then `fill` copies of x, then (unless at end)
// the entries from the position on; the old entries are destroyed (names and
// index lists released) and the old block freed.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fixture scaffolding: copies share the source's data pointer.
static int g_copies, g_dtors;
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<void**>(this) = *reinterpret_cast<void* const*>(&other); }
CCharString::~CCharString() { ++g_dtors; }

typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long, Info> Entry;
typedef _STL::vector<Entry> EntryVector;
static CDefClassBase* FABLE_FASTCALL Alloc() { return 0; }
static void* Raw(const CCharString& s) { return *reinterpret_cast<void* const*>(&s); }
// Entry k: key k, name 0x100 + k, one global index 1000 + k (its own block).
static void Fill(Entry& e, unsigned long k)
{
    e.first = k;
    *reinterpret_cast<unsigned long*>(&e.second.Name) = 0x100 + k;
    e.second.AllocFunc = &Alloc;
    unsigned long* block = (unsigned long*)malloc(sizeof(unsigned long));
    *block = 1000 + k;
    e.second.GlobalIndices._M_start = block;
    e.second.GlobalIndices._M_finish = block + 1;
    e.second.GlobalIndices._M_end_of_storage._M_data = block + 1;
}
static bool Is(const Entry& e, unsigned long k)
{
    return e.first == k && Raw(e.second.Name) == (void*)(0x100 + k) && e.second.AllocFunc == &Alloc &&
           e.second.GlobalIndices._M_finish == e.second.GlobalIndices._M_start + 1 &&
           *e.second.GlobalIndices._M_start == 1000 + k;
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
    unsigned long* oldIndex0 = old[0].second.GlobalIndices._M_start;
    v->_M_start = old;
    v->_M_finish = old + 2;
    v->_M_end_of_storage._M_data = old + 2;
    v->_M_insert_overflow(old + 1, *x, tag, 1, false);
    Entry* n = v->_M_start;
    if (n == old || v->_M_finish != n + 3 || v->_M_end_of_storage._M_data != n + 4) { puts("grow"); return 1; }
    if (!Is(n[0], 0) || !Is(n[1], 9) || !Is(n[2], 1) || n[0].second.GlobalIndices._M_start == oldIndex0) { puts("order"); return 1; }
    if (g_copies != 3 || g_dtors != 2) { printf("counts %d %d\n", g_copies, g_dtors); return 1; }

    // At-end insert of three copies: capacity 3 + max(3, 3) = 6, the tail
    // copy is skipped, the three old entries destroyed.
    g_copies = g_dtors = 0;
    v->_M_insert_overflow(v->_M_finish, *x, tag, 3, true);
    Entry* m = v->_M_start;
    if (m == n || v->_M_finish != m + 6 || v->_M_end_of_storage._M_data != m + 6) { puts("fill"); return 1; }
    if (!Is(m[0], 0) || !Is(m[1], 9) || !Is(m[2], 1) || !Is(m[3], 9) || !Is(m[4], 9) || !Is(m[5], 9)) { puts("fillorder"); return 1; }
    if (g_copies != 6 || g_dtors != 3) { printf("fillcounts %d %d\n", g_copies, g_dtors); return 1; }
    puts("FABLETLC_VECTOR_DEFCLASSENTRY_INSERT_OVERFLOW_009B8DD0_PASS");
    return 0;
}