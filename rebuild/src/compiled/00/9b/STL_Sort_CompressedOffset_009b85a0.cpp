// Native STLport Sort for compressed-definition index/offset pairs at 0x009B85A0.
#include "fable_vector_map.h"
#include "fable_stlport_algo_impl.h"

typedef _STL::pair<unsigned long, unsigned long> IndexEntry;
template void FABLE_FASTCALL _STL::sort(IndexEntry* first, IndexEntry* last,
                                       CKeyPairCompareLess<unsigned long, unsigned long> comp);
