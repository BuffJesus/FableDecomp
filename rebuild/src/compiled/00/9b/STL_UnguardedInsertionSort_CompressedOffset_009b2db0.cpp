// Native STLport UnguardedInsertionSort for compressed-definition index/offset pairs at 0x009B2DB0.
#include "fable_vector_map.h"
#include "fable_stlport_algo.h"
typedef _STL::pair<unsigned long, unsigned long> Entry;
typedef CKeyPairCompareLess<unsigned long, unsigned long> Less;
#include "fable_stlport_unguarded_insertion_sort.h"
template void FABLE_FASTCALL _STL::__unguarded_insertion_sort_aux(Entry*,Entry*,Entry*,Less);
