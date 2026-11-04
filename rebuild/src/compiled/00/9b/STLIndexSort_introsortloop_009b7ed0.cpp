// Definition-name CRC/global-index introsort at 0x009B7ED0; called by 0x009B8520.
#include "fable_vector_map.h"
#include "fable_stlport_introsort.h"
typedef _STL::pair<unsigned long, long> Entry;
template void FABLE_FASTCALL _STL::__introsort_loop(Entry*, Entry*, Entry*, int,
    CKeyPairCompareLess<unsigned long, long>);