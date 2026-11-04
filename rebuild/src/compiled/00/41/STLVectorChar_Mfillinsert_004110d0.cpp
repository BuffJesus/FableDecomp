// _STL::vector<char>::_M_fill_insert(char*, size_t, const char&) @ 0x004110D0
// (STLport, POD form; resize 0x004122D0 appends through it; the growth path
// calls _M_insert_overflow 0x0040F6B0).
#include "fable_stlport_vector_impl.h"

template void _STL::vector<char>::_M_fill_insert(char* __position, size_t __n, const char& __x);