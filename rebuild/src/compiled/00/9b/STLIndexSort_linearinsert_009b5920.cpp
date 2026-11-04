#include "fable_vector_map.h"
#include "fable_stlport_linear_insert.h"
typedef _STL::pair<unsigned long,long> Entry;
typedef CKeyPairCompareLess<unsigned long,long> Less;
template void FABLE_FASTCALL _STL::__linear_insert(Entry*,Entry*,Entry,Less);