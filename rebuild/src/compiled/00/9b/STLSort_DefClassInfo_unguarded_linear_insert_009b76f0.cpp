// _STL::__unguarded_linear_insert<pair<unsigned long, CDefinitionManager::
// CDefClassInfo>*, pair<...>, CKeyPairCompareLess<...> > @ 0x009B76F0
// (fastcall; the value and the comparator by value on the stack), from
// __linear_insert 0x009B8C00 when the value does not go first.
#include "fable_stlport_unguarded_linear_insert.h"
#include "fable_definition_manager.h"

typedef _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo> DefClassEntry;

template void FABLE_FASTCALL _STL::__unguarded_linear_insert<DefClassEntry*, DefClassEntry,
    CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> >(
    DefClassEntry* __last, DefClassEntry __val,
    CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> __comp);