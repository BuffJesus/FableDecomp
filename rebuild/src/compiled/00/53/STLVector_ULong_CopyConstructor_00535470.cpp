// _STL::vector<unsigned long>::vector(vector const&) @ 0x00535470 (a shared
// instantiation; AddDefClass 0x009B0AC0 reaches it copying a CDefClassInfo's
// GlobalIndices). The body is STLport's, in fable_stlport_vector_impl.h:
// allocate exactly size() elements, then copy them as raw memory.
#include "fable_stlport_vector_impl.h"

template _STL::vector<unsigned long>::vector(const _STL::vector<unsigned long>& __x);