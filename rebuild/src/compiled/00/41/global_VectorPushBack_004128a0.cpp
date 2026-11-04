// _STL::vector<CWideString>::push_back(const CWideString&) @ 0x004128A0 (STLport;
// CStartupDefinitionManager::InitAndCompilePlatformSpecific 0x004336BC builds
// its path lists with it). The body (copy into the spare slot, or grow the
// storage by one through _M_insert_overflow) is STLport's, in
// fable_stlport_vector_impl.h.
#include "fable_string.h"
#include "fable_stlport_vector_impl.h"

#include <new>

template void _STL::vector<CWideString>::push_back(const CWideString& __x);
