// Behaviour of CVectorMap<CCharString, CSymbolInfo, ...>::DeallocateUnusedMemory
// 0x009E7C70: with spare capacity the entries are copied (key, group, entry
// name, value) into a block of exactly size() elements, the map releases its
// own storage (DeallocateMemory) and then owns the new block; a map that is
// already full is left alone.
#include "fable_symbol_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fixture scaffolding: a string is its data pointer; count copies and frees.
static int g_copies, g_frees;
static long Raw(const CCharString& s) { return *reinterpret_cast<const long*>(&s); }
CCharString::CCharString(const CCharString& other) { ++g_copies; *reinterpret_cast<long*>(this) = Raw(other); }
CCharString::~CCharString() { ++g_frees; }
CCharString& CCharString::operator=(const CCharString& other) { *reinterpret_cast<long*>(this) = Raw(other); return *this; }

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
typedef CVectorMap<CCharString, CSymbolMap::CSymbolInfo, CCompareString<CSymbolMap::CSymbolInfo> > Map;
static int g_released;
static Entry* g_releasedBlock;
static int g_sizeAtRelease;
template<> void CArrayVector<Entry>::DeallocateMemory()
{
    ++g_released;
    g_releasedBlock = this->_M_start;
    g_sizeAtRelease = (int)this->size();
    free(this->_M_start);
    this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0;
}

int main()
{
    unsigned char store[sizeof(Map)];
    memset(store, 0, sizeof(store));
    Map* map = reinterpret_cast<Map*>(store);
    Entry* block = (Entry*)malloc(4 * sizeof(Entry));
    for (int i = 0; i < 2; ++i)
    {
        *reinterpret_cast<long*>(&block[i].first) = 10 + i;
        *reinterpret_cast<long*>(&block[i].second.GroupName) = 20 + i;
        *reinterpret_cast<long*>(&block[i].second.EntryName) = 30 + i;
        block[i].second.Val = 40 + i;
    }
    map->_M_start = block;
    map->_M_finish = block + 2;
    map->_M_end_of_storage._M_data = block + 4;

    map->DeallocateUnusedMemory();
    if (g_released != 1 || g_releasedBlock != block || g_sizeAtRelease != 2) { puts("release"); return 1; }
    Entry* now = map->_M_start;
    if (now == 0 || now == block || map->_M_finish != now + 2 || map->_M_end_of_storage._M_data != now + 2) { puts("storage"); return 1; }
    for (int i = 0; i < 2; ++i)
        if (Raw(now[i].first) != 10 + i || Raw(now[i].second.GroupName) != 20 + i ||
            Raw(now[i].second.EntryName) != 30 + i || now[i].second.Val != 40 + i) { puts("copy"); return 1; }
    if (g_copies != 6 || g_frees != 0) { printf("copies %d frees %d\n", g_copies, g_frees); return 1; }

    map->DeallocateUnusedMemory();
    if (g_released != 1 || map->_M_start != now || g_copies != 6) { puts("full"); return 1; }
    puts("FABLETLC_CVECTORMAP_SYMBOL_DEALLOCATEUNUSEDMEMORY_009E7C70_PASS");
    return 0;
}