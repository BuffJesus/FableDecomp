// Native STLport UnguardedLinearInsert for compressed-definition index/offset pairs at 0x009B1B00.
#include "fable_vector_map.h"
#include "fable_stlport_linear_insert.h"
typedef _STL::pair<unsigned long, unsigned long> Entry;
typedef CKeyPairCompareLess<unsigned long, unsigned long> Less;
#include "fable_stlport_unguarded_linear_insert.h"
template void FABLE_FASTCALL _STL::__unguarded_linear_insert(Entry*,Entry,Less);
