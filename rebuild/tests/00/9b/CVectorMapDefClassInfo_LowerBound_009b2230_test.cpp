// Behaviour of CVectorMap<unsigned long, CDefClassInfo, ...>::LowerBound 0x009B2230:
// on the sorted entries, returns the first entry whose key is not below the
// given key (the end when every key is smaller; the first of equal keys).
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long, Info> Entry;
typedef CVectorMap<unsigned long, Info, CKeyPairCompareLess<unsigned long, Info> > Map;

static unsigned char g_mapStore[sizeof(Map)];
static unsigned char g_entryStore[7][sizeof(Entry)];

static int Expect(Map* map, unsigned long key, long wantIndex)
{
    Entry* got = map->LowerBound(key);
    if (got != map->_M_start + wantIndex)
    {
        printf("key %lu: got index %ld want %ld\n", key, (long)(got - map->_M_start), wantIndex);
        return 1;
    }
    return 0;
}

int main()
{
    Map* map = reinterpret_cast<Map*>(g_mapStore);
    Entry* entries = reinterpret_cast<Entry*>(g_entryStore);
    const unsigned long keys[7] = { 2, 4, 4, 4, 9, 0x80000000UL, 0xFFFFFFF0UL };
    for (int i = 0; i < 7; ++i) entries[i].first = keys[i];
    map->_M_start = entries;
    map->_M_finish = entries + 7;

    if (Expect(map, 0, 0)) return 1;
    if (Expect(map, 2, 0)) return 1;
    if (Expect(map, 3, 1)) return 1;
    if (Expect(map, 4, 1)) return 1;          // first of the equal keys
    if (Expect(map, 5, 4)) return 1;
    if (Expect(map, 9, 4)) return 1;
    if (Expect(map, 10, 5)) return 1;
    if (Expect(map, 0x80000000UL, 5)) return 1; // keys compare unsigned
    if (Expect(map, 0xFFFFFFF0UL, 6)) return 1;
    if (Expect(map, 0xFFFFFFFFUL, 7)) return 1; // past every key: end

    for (int n = 0; n <= 7; ++n)
    {
        map->_M_finish = entries + n;
        for (unsigned long k = 0; k < 12; ++k)
        {
            long want = 0;
            while (want < n && entries[want].first < k) ++want;
            if (Expect(map, k, want)) return 1;
        }
    }
    puts("FABLETLC_CVECTORMAP_DEFCLASSINFO_LOWERBOUND_009B2230_PASS");
    return 0;
}