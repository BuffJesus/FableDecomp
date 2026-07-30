#include <stdio.h>
#include <string.h>

#include "fable_vector_map.h"

FableVectorMapEntry* FableVectorMapStorage::LowerBound(
    const fable_u32& key)
{
    FableVectorMapEntry* position = begin_;
    while (position != end_ && position->key < key)
        ++position;
    return position;
}

int main()
{
    FableVectorMapRecovered map;
    memset(&map, 0, sizeof(map));

    FableVectorMapEntry entries[] = {
        {10, 100},
        {20, 200},
        {40, 400},
    };
    map.entries_.begin_ = entries;
    map.entries_.end_ = entries + 3;

    if (map.LookupValue(10) != 100)
        return 1;
    if (map.LookupValue(20) != 200)
        return 2;
    if (map.LookupValue(40) != 400)
        return 3;
    if (map.LookupValue(5) != 0)
        return 4;
    if (map.LookupValue(30) != 0)
        return 5;
    if (map.LookupValue(50) != 0)
        return 6;

    map.entries_.begin_ = entries;
    map.entries_.end_ = entries;
    if (map.LookupValue(10) != 0)
        return 7;

    puts("FABLETLC_VECTOR_MAP_LOOKUP_BEHAVIOR PASS");
    return 0;
}
