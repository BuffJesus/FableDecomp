// Native STLport LinearInsert for compressed-definition index/offset pairs at 0x009B46B0.
#include "fable_vector_map.h"
#include "fable_stlport_linear_insert.h"
typedef _STL::pair<unsigned long, unsigned long> Entry;
typedef CKeyPairCompareLess<unsigned long, unsigned long> Less;
template void FABLE_FASTCALL _STL::__linear_insert(Entry*,Entry*,Entry,Less);
