// Behaviour of _STL::__uninitialized_fill_n over pair<CCharString, CSymbolInfo>
// 0x009E62E0: n copies of x are copy-constructed into consecutive 16-byte
// slots from `first`; the return is one past the last; n == 0 copies nothing.
// Call the landed instantiation, not a copy compiled from the header bodies.
#define FABLE_STLPORT_DECLARATIONS_ONLY
#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>

// Fixture scaffolding: copies share the source's data pointer.
static int g_copies;
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<void**>(this) = *reinterpret_cast<void* const*>(&other); }
CCharString::~CCharString() {}

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
static void* Raw(const CCharString& s) { return *reinterpret_cast<void* const*>(&s); }
static void Set(CCharString& s, unsigned long v) { *reinterpret_cast<unsigned long*>(&s) = v; }

int main()
{
    static unsigned char src[sizeof(Entry)];
    static unsigned char dst[4 * sizeof(Entry)];
    Entry* x = reinterpret_cast<Entry*>(src);
    Set(x->first, 0x11);
    Set(x->second.GroupName, 0x22);
    Set(x->second.EntryName, 0x33);
    x->second.Val = -5;
    memset(dst, 0xCC, sizeof(dst));
    Entry* d = reinterpret_cast<Entry*>(dst);
    _STL::__false_type tag;
    Entry* end = _STL::__uninitialized_fill_n(d, (size_t)3, *x, tag);
    if (end != d + 3 || g_copies != 9) { puts("range"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (Raw(d[i].first) != (void*)0x11 || Raw(d[i].second.GroupName) != (void*)0x22 ||
            Raw(d[i].second.EntryName) != (void*)0x33 || d[i].second.Val != -5) { puts("copy"); return 1; }
    if (dst[3 * sizeof(Entry)] != 0xCC) { puts("overrun"); return 1; }
    if (_STL::__uninitialized_fill_n(d, (size_t)0, *x, tag) != d || g_copies != 9) { puts("empty"); return 1; }
    puts("FABLETLC_STL_UNINITIALIZED_FILL_N_SYMBOLENTRY_009E62E0_PASS");
    return 0;
}
