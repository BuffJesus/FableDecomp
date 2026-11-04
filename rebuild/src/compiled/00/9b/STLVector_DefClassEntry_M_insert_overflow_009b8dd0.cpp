// _STL::vector<pair<unsigned long, CDefinitionManager::CDefClassInfo> >::_M_insert_overflow
// (pair*, pair const&, __false_type const&, unsigned int, bool) @ 0x009B8DD0
// (retail DefinitionManager.cpp, from push_back 0x009B9170). The body is
// STLport's, in fable_stlport_vector_impl.h.
#include "fable_definition_manager.h"
#include "fable_stlport_vector_impl.h"

#include <new>

typedef _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo> DefClassEntry;
template void _STL::vector<DefClassEntry>::_M_insert_overflow(DefClassEntry* __position, const DefClassEntry& __x,
                                                              const _STL::__false_type&, size_t __fill_len,
                                                              bool __atend);