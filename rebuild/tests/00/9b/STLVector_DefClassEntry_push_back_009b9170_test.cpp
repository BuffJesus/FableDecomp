// Behaviour of vector<pair<unsigned long, CDefinitionManager::CDefClassInfo> >::push_back
// 0x009B9170: with spare capacity the entry is copy-constructed at the end (key,
// class name, allocator, and its own copy of the global-index list) and the end
// moves one 24-byte element; when full it hands the end, the entry, fill count
// 1 and at-end to _M_insert_overflow without touching the vector itself.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fixture scaffolding: copies share the source's data pointer.
static int g_copies;
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<void**>(this) = *reinterpret_cast<void* const*>(&other); }
CCharString::~CCharString() {}

typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long, Info> Entry;
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
static CDefClassBase* FABLE_FASTCALL Alloc() { return 0; }

int main()
{
    static unsigned char buffer[2 * sizeof(Entry)];
    memset(buffer, 0xCC, sizeof(buffer));
    unsigned long indices[2] = { 7, 11 };
    unsigned char source[sizeof(Entry)];
    Entry* x = reinterpret_cast<Entry*>(source);
    x->first = 0xC0DE;
    *reinterpret_cast<void**>(&x->second.Name) = (void*)0x1111;
    x->second.AllocFunc = &Alloc;
    x->second.GlobalIndices._M_start = indices;
    x->second.GlobalIndices._M_finish = indices + 2;
    x->second.GlobalIndices._M_end_of_storage._M_data = indices + 2;

    unsigned char vstore[sizeof(EntryVector)];
    EntryVector* v = reinterpret_cast<EntryVector*>(vstore);
    Entry* base = reinterpret_cast<Entry*>(buffer);
    v->_M_start = base;
    v->_M_finish = base;
    v->_M_end_of_storage._M_data = base + 2;

    v->push_back(*x);
    if (v->_M_finish != base + 1 || g_copies != 1 || g_overflows != 0) { puts("append"); return 1; }
    const Info& c = base[0].second;
    if (base[0].first != 0xC0DE || *reinterpret_cast<void* const*>(&c.Name) != (void*)0x1111 || c.AllocFunc != &Alloc) { puts("copy"); return 1; }
    if (c.GlobalIndices._M_start == indices || c.GlobalIndices._M_finish != c.GlobalIndices._M_start + 2 ||
        c.GlobalIndices._M_start[0] != 7 || c.GlobalIndices._M_start[1] != 11) { puts("indices"); return 1; }
    if (buffer[sizeof(Entry)] != 0xCC) { puts("overrun"); return 1; }
    free(c.GlobalIndices._M_start);

    v->_M_finish = base + 2;
    v->push_back(*x);
    if (v->_M_finish != base + 2 || g_overflows != 1 || g_overflowThis != v || g_overflowPos != base + 2 ||
        g_overflowValue != x || g_fill != 1 || !g_atEnd || g_copies != 1) { puts("overflow"); return 1; }
    puts("FABLETLC_VECTOR_DEFCLASSENTRY_PUSHBACK_009B9170_PASS");
    return 0;
}