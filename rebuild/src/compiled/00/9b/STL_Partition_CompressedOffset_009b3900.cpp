// Native STLport Partition for compressed-definition index/offset pairs at 0x009B3900.
#include "fable_vector_map.h"
#include "fable_stlport_partition.h"
typedef _STL::pair<unsigned long, unsigned long> Entry;
template Entry* FABLE_FASTCALL _STL::__unguarded_partition(Entry*, Entry*, Entry,
    CKeyPairCompareLess<unsigned long, unsigned long>);
