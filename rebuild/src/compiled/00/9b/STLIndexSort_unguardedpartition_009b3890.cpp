// Definition-name CRC/global-index partition at 0x009B3890.
#include "fable_vector_map.h"
#include "fable_stlport_partition.h"
typedef _STL::pair<unsigned long, long> Entry;
template Entry* FABLE_FASTCALL _STL::__unguarded_partition(Entry*, Entry*, Entry,
    CKeyPairCompareLess<unsigned long, long>);