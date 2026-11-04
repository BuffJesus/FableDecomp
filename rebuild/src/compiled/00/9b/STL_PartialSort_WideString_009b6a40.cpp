// STLport __partial_sort<CWideString*, less<CWideString>> @ 0x009B6A40.
// Introsort calls this path when its depth limit reaches zero.
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"

namespace _STL {
template<class I, class T, class C, class D>
void FABLE_FASTCALL __pop_heap(I first, I last, I result, T value, C comparator, D*) {
    *result = *first;
    __adjust_heap(first, D(0), D(last - first), value, comparator);
}

template<class I, class T, class C>
void FABLE_FASTCALL __partial_sort(I first, I middle, I last, T*, C comparator) {
    __make_heap(first, middle, comparator, (T*)0, (int*)0);
    for (I it = middle; it < last; ++it)
        if (comparator(*it, *first))
            __pop_heap(first, middle, it, T(*it), comparator, (int*)0);
    sort_heap(first, middle, comparator);
}
}

template void FABLE_FASTCALL _STL::__partial_sort(
    CWideString* first, CWideString* middle, CWideString* last,
    CWideString* tag, _STL::less<CWideString> comparator);
