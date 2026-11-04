// STLport sort of the definition-name CRC/global-index map at 0x009B8520.
// Called by LoadBinaryDefinitions (0x009B00BB); 115 bytes including both returns.
// The final insertion pass expands inline for these 8-byte entries.
#include "fable_vector_map.h"
#include "fable_stlport_algo_impl.h"

typedef _STL::pair<unsigned long, long> IndexEntry;
template void FABLE_FASTCALL _STL::sort(IndexEntry* first, IndexEntry* last,
                                       CKeyPairCompareLess<unsigned long, long> comp);