// _STL::__insertion_sort<pair<unsigned long, CDefinitionManager::CDefClassInfo>*,
// CKeyPairCompareLess<unsigned long, CDefClassInfo> >(first, last, comp) @
// 0x009B8F70 (retail DefinitionManager.cpp, from the final insertion pass
// 0x009B91F0). The body is STLport's, in fable_stlport_insertion_sort.h; each
// element's copy expands the vector<unsigned long> copy constructor.
#include "fable_definition_manager.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_insertion_sort.h"

typedef _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo> DefClassEntry;
template void FABLE_FASTCALL _STL::__insertion_sort(
    DefClassEntry* __first, DefClassEntry* __last,
    CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> __comp);