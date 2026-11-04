// Behaviour of _STL::__destroy_aux over pair<CCharString, CSymbolInfo>
// 0x006FBCC0: every element of [first, last) is destroyed in order, each as
// entry name, group name, then key; an empty range destroys nothing.
// Call the landed instantiation, not a copy compiled from the header bodies.
#define FABLE_STLPORT_DECLARATIONS_ONLY
#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>

// Fixture scaffolding: record which strings are destroyed, in order.
static const void* g_order[16];
static int g_count;
CCharString::~CCharString() { g_order[g_count++] = this; }

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;

int main()
{
    static unsigned char buffer[2 * sizeof(Entry)];
    Entry* e = reinterpret_cast<Entry*>(buffer);
    _STL::__false_type tag;
    _STL::__destroy_aux(e, e, tag);
    if (g_count != 0) { puts("empty"); return 1; }
    _STL::__destroy_aux(e, e + 2, tag);
    if (g_count != 6) { puts("count"); return 1; }
    for (int i = 0; i < 2; ++i)
        if (g_order[3 * i] != &e[i].second.EntryName || g_order[3 * i + 1] != &e[i].second.GroupName ||
            g_order[3 * i + 2] != &e[i].first) { puts("order"); return 1; }
    puts("FABLETLC_STL_DESTROY_AUX_SYMBOLENTRY_006FBCC0_PASS");
    return 0;
}
