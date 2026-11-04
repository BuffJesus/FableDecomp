// _STL::vector<pair<unsigned long, CDefinitionManager::CDefClassInfo> >::push_back
// (pair const&) @ 0x009B9170 (retail DefinitionManager.cpp, from AddDefClass
// 0x009B0AC0 through CVectorMap::Insert). The body is STLport's, in
// fable_stlport_vector_impl.h.
#include "fable_definition_manager.h"
#include "fable_stlport_vector_impl.h"

#include <new>

typedef _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo> DefClassEntry;
template void _STL::vector<DefClassEntry>::push_back(const DefClassEntry& __x);