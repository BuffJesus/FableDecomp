// Native STLport Introsort for compressed-definition index/offset pairs at 0x009B7FA0.
#include "fable_vector_map.h"
#include "fable_stlport_introsort.h"
typedef _STL::pair<unsigned long, unsigned long> Entry;
template void FABLE_FASTCALL _STL::__introsort_loop(Entry*, Entry*, Entry*, int,
    CKeyPairCompareLess<unsigned long, unsigned long>);
