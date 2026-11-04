// _STL::sort<pair<unsigned long, CDefinitionManager::CDefClassInfo>*,
// CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> >
// (first, last, comp) @ 0x009B9490 (retail DefinitionManager.cpp, from
// CVectorMap::Sort in AddDefClass 0x009B0AC0). The body is STLport's, in
// fable_stlport_algo_impl.h; here the final insertion pass stays a call.
#include "fable_definition_manager.h"
#include "fable_stlport_algo_impl.h"

typedef _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo> DefClassEntry;
template void FABLE_FASTCALL _STL::sort(DefClassEntry* __first, DefClassEntry* __last,
                                        CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> __comp);