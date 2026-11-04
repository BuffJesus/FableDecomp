// Behaviour of CSymbolMap::UpdateDirty() const 0x009E5C30: a dirty SLONGMap is
// sorted over [start, finish) with its comparator and marked clean; the map's
// storage is trimmed (DeallocateUnusedMemory on SLONGMap) whether or not it
// was dirty, after the sort.
#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
typedef CVectorMap<CCharString, CSymbolMap::CSymbolInfo, CCompareString<CSymbolMap::CSymbolInfo> > Map;
static int g_step;
static int g_sorted, g_trimmed;
static Entry* g_first;
static Entry* g_last;
static const void* g_trimThis;
template<> void FABLE_FASTCALL _STL::sort(Entry* first, Entry* last, CCompareString<CSymbolMap::CSymbolInfo>)
{
    g_sorted = ++g_step;
    g_first = first;
    g_last = last;
}
template<> void Map::DeallocateUnusedMemory()
{
    g_trimmed = ++g_step;
    g_trimThis = this;
}

int main()
{
    static unsigned char store[sizeof(CSymbolMap)];
    memset(store, 0, sizeof(store));
    CSymbolMap* map = reinterpret_cast<CSymbolMap*>(store);
    Entry* base = reinterpret_cast<Entry*>(0x1000);
    map->SLONGMap._M_start = base;
    map->SLONGMap._M_finish = base + 3;
    map->SLONGMap.Dirty = true;

    map->UpdateDirty();
    if (g_sorted != 1 || g_first != base || g_last != base + 3) { puts("sort"); return 1; }
    if (map->SLONGMap.Dirty) { puts("clean"); return 1; }
    if (g_trimmed != 2 || g_trimThis != &map->SLONGMap) { puts("trim"); return 1; }

    g_step = 0; g_sorted = 0; g_trimmed = 0;
    map->UpdateDirty();
    if (g_sorted != 0 || g_trimmed != 1 || map->SLONGMap.Dirty) { puts("clean path"); return 1; }
    puts("FABLETLC_CSYMBOLMAP_UPDATEDIRTY_009E5C30_PASS");
    return 0;
}