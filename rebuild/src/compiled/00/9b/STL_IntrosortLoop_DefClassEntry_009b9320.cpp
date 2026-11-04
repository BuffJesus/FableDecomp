// _STL::__introsort_loop<pair<unsigned long, CDefinitionManager::CDefClassInfo>*, pair<...>,
// int, CKeyPairCompareLess<unsigned long, CDefClassInfo> >(first, last, _Tp*,
// depth_limit, comp) @ 0x009B9320 (retail DefinitionManager.cpp, from sort
// 0x009B9490). The body is STLport's, in fable_stlport_introsort.h; the pivot
// copy expands the vector<unsigned long> copy constructor
// (fable_stlport_vector_impl.h).
#include "fable_definition_manager.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_introsort.h"

typedef _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo> DefClassEntry;
template void FABLE_FASTCALL _STL::__introsort_loop(
    DefClassEntry* __first, DefClassEntry* __last, DefClassEntry*, int __depth_limit,
    CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> __comp);