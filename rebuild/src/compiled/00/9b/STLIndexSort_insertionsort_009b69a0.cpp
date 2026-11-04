#include "fable_vector_map.h"
#include "fable_stlport_algo.h"
typedef _STL::pair<unsigned long,long> Entry;
typedef CKeyPairCompareLess<unsigned long,long> Less;
#include "fable_stlport_insertion_sort.h"
template void FABLE_FASTCALL _STL::__insertion_sort(Entry*,Entry*,Less);