// _STL::vector<char>::reserve(size_t) @ 0x00997330 (retail MemoryDataOutputStream
// unit; CMemoryDataOutputStream::ReserveSize 0x00996BC0 tail-calls it). The body
// is STLport's, in fable_stlport_vector_impl.h: grow to exactly n bytes when the
// capacity is smaller, moving the bytes over and releasing the old block.
#include "fable_stlport_vector_impl.h"

#include <new>

template void _STL::vector<char>::reserve(size_t __n);