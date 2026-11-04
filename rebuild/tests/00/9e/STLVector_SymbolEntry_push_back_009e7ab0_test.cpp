// Behaviour of vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::push_back
// 0x009E7AB0: with spare capacity the entry is copy-constructed at the end
// (key, group, entry name, value) and the end moves one 16-byte element; when
// full it hands the end, the entry, fill count 1 and at-end to
// _M_insert_overflow without touching the vector itself.
#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>

// Fixture scaffolding: copies share the source's data pointer.
static int g_copies;
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<void**>(this) = *reinterpret_cast<void* const*>(&other); }
CCharString::~CCharString() {}

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
typedef _STL::vector<Entry> EntryVector;
static int g_overflows;
static const void* g_overflowThis;
static Entry* g_overflowPos;
static const Entry* g_overflowValue;
static size_t g_fill;
static bool g_atEnd;
template<> void EntryVector::_M_insert_overflow(Entry* pos, const Entry& x, const _STL::__false_type&, size_t fill, bool atEnd)
{
    ++g_overflows;
    g_overflowThis = this;
    g_overflowPos = pos;
    g_overflowValue = &x;
    g_fill = fill;
    g_atEnd = atEnd;
}

static void* Raw(const CCharString& s) { return *reinterpret_cast<void* const*>(&s); }

int main()
{
    static unsigned char buffer[2 * sizeof(Entry)];
    memset(buffer, 0xCC, sizeof(buffer));
    unsigned char source[sizeof(Entry)];
    Entry* x = reinterpret_cast<Entry*>(source);
    *reinterpret_cast<void**>(&x->first) = (void*)0x1111;
    *reinterpret_cast<void**>(&x->second.GroupName) = (void*)0x2222;
    *reinterpret_cast<void**>(&x->second.EntryName) = (void*)0x3333;
    x->second.Val = 42;

    unsigned char vstore[sizeof(EntryVector)];
    EntryVector* v = reinterpret_cast<EntryVector*>(vstore);
    Entry* base = reinterpret_cast<Entry*>(buffer);
    v->_M_start = base;
    v->_M_finish = base;
    v->_M_end_of_storage._M_data = base + 2;

    v->push_back(*x);
    if (v->_M_finish != base + 1 || g_copies != 3 || g_overflows != 0) { puts("append"); return 1; }
    if (Raw(base[0].first) != (void*)0x1111 || Raw(base[0].second.GroupName) != (void*)0x2222 ||
        Raw(base[0].second.EntryName) != (void*)0x3333 || base[0].second.Val != 42) { puts("copy"); return 1; }
    if (buffer[sizeof(Entry)] != 0xCC) { puts("overrun"); return 1; }

    v->push_back(*x);
    v->push_back(*x);
    if (v->_M_finish != base + 2 || g_overflows != 1 || g_overflowThis != v || g_overflowPos != base + 2 ||
        g_overflowValue != x || g_fill != 1 || !g_atEnd || g_copies != 6) { puts("overflow"); return 1; }
    puts("FABLETLC_VECTOR_SYMBOLENTRY_PUSHBACK_009E7AB0_PASS");
    return 0;
}
