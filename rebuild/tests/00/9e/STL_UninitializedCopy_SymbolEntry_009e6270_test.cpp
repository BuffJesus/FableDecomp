// Behaviour of _STL::__uninitialized_copy over pair<CCharString, CSymbolInfo>
// 0x009E6270: each source entry is copy-constructed (key, group, entry name,
// value) into consecutive 16-byte slots from `result`; the return is one past
// the last copy; an empty range copies nothing and returns `result`.
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
    static unsigned char src[2 * sizeof(Entry)];
    static unsigned char dst[3 * sizeof(Entry)];
    Entry* s = reinterpret_cast<Entry*>(src);
    for (int i = 0; i < 2; ++i)
    {
        Set(s[i].first, 0x100 + i);
        Set(s[i].second.GroupName, 0x200 + i);
        Set(s[i].second.EntryName, 0x300 + i);
        s[i].second.Val = 10 + i;
    }
    memset(dst, 0xCC, sizeof(dst));
    Entry* d = reinterpret_cast<Entry*>(dst);
    _STL::__false_type tag;
    Entry* end = _STL::__uninitialized_copy(s, s + 2, d, tag);
    if (end != d + 2 || g_copies != 6) { puts("range"); return 1; }
    for (int i = 0; i < 2; ++i)
        if (Raw(d[i].first) != (void*)(0x100 + i) || Raw(d[i].second.GroupName) != (void*)(0x200 + i) ||
            Raw(d[i].second.EntryName) != (void*)(0x300 + i) || d[i].second.Val != 10 + i) { puts("copy"); return 1; }
    if (dst[2 * sizeof(Entry)] != 0xCC) { puts("overrun"); return 1; }
    if (_STL::__uninitialized_copy(s, s, d, tag) != d || g_copies != 6) { puts("empty"); return 1; }
    puts("FABLETLC_STL_UNINITIALIZED_COPY_SYMBOLENTRY_009E6270_PASS");
    return 0;
}
