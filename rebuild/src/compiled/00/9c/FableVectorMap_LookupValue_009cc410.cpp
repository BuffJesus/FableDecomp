#include "fable_vector_map.h"

fable_u32 FableVectorMapRecovered::LookupValue(fable_u32 key)
{
    FableVectorMapStorage* storage = &entries_;
    FableVectorMapEntry* position =
        storage->LowerBound(key);
    FableVectorMapEntry* end = storage->end_;

    if (position == end || key < position->key)
        position = end;

    if (position != entries_.end_)
        return position->value;
    return 0;
}
