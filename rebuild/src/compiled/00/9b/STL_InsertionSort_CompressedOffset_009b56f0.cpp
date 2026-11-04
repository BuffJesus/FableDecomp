// Native STLport InsertionSort for compressed-definition index/offset pairs at 0x009B56F0.
#include "fable_vector_map.h"
#include "fable_stlport_algo.h"
typedef _STL::pair<unsigned long, unsigned long> Entry;
typedef CKeyPairCompareLess<unsigned long, unsigned long> Less;
#include "fable_stlport_insertion_sort.h"
template void FABLE_FASTCALL _STL::__insertion_sort(Entry*,Entry*,Less);
