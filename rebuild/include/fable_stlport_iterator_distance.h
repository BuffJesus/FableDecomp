#pragma once

#include "fable_stlport_vector.h"

namespace _STL {
// STLport 4.x input-iterator overload. References preserve the original
// iterator/category ABI; the definition lives in each required native TU.
template<class I>
ptrdiff_t __cdecl __distance(const I& first, const I& last,
                            const input_iterator_tag& category);

// Historical generic class-iterator traits. Pointer specializations and
// random-access overloads are not reconstructed by this bounded surface.
template<class I> struct iterator_traits {
    typedef typename I::iterator_category iterator_category;
    typedef typename I::value_type value_type;
    typedef typename I::difference_type difference_type;
    typedef typename I::pointer pointer;
    typedef typename I::reference reference;
};

template<class I>
inline typename iterator_traits<I>::difference_type __cdecl
distance(const I& first, const I& last)
{
    return __distance(first, last, typename iterator_traits<I>::iterator_category());
}
}
