// Front-end history deque map-slot allocator @ 0x0059A9E0.
//
// The map is an array of pointers to 32-element history blocks.  Fable's
// allocator is malloc-backed and returns null for an empty request; the
// standard allocator hint parameter is accepted but unused.

// Template bodies compile at the end of the TU, so size optimisation stays on.
#pragma optimize("s", on)
#include <stdlib.h>

template<class T>
class FrontEndHistoryDequeMapAllocator
{
public:
    T* Allocate(unsigned long count, const void*)
    {
        return count ? static_cast<T*>(malloc(count * sizeof(T))) : 0;
    }
};

template class FrontEndHistoryDequeMapAllocator<void*>;
