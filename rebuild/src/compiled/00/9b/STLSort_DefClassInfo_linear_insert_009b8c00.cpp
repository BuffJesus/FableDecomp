// _STL::__linear_insert<pair<unsigned long, CDefinitionManager::CDefClassInfo>*,
// pair<...>, CKeyPairCompareLess<...> > @ 0x009B8C00 (fastcall; the value and
// the comparator by value on the stack). __insertion_sort 0x009B8F70 calls it
// for each element of CDefinitionManager::DefClassInfo when AddDefClass sorts.
#include "fable_stlport_linear_insert.h"
#include "fable_definition_manager.h"

typedef _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo> DefClassEntry;

template void FABLE_FASTCALL _STL::__linear_insert<DefClassEntry*, DefClassEntry,
    CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> >(
    DefClassEntry* __first, DefClassEntry* __last, DefClassEntry __val,
    CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> __comp);