// STLport Hoare partition<CWideString*, less<CWideString>> @ 0x009B3580.
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_algo.h"

namespace _STL {
template<class I, class T, class C>
I FABLE_FASTCALL __unguarded_partition(I first, I last, T pivot, C comparator) {
    for (;;) {
        while (comparator(*first, pivot)) ++first;
        --last;
        while (comparator(pivot, *last)) --last;
        if (!(first < last)) return first;
        {
            T temp = *first;
            *first = *last;
            *last = temp;
        }
        ++first;
    }
}
}

template CWideString* FABLE_FASTCALL _STL::__unguarded_partition(
    CWideString* first, CWideString* last, CWideString pivot,
    _STL::less<CWideString> comparator);
