#include "fable_game_text_bank.h"
#include "fable_stlport_iterator_distance.h"

namespace _STL {
// Original STLport input-iterator algorithm; retail entry 0x009D7550.
template<class I>
inline ptrdiff_t __cdecl __distance(const I& first, const I& last,
                                  const input_iterator_tag&)
{
    ptrdiff_t count = 0;
    I current(first);
    while (current != last) {
        ++current;
        ++count;
    }
    return count;
}

template ptrdiff_t __cdecl __distance<_Rb_tree_iterator<CBankFileAsyncData*> >(
    const _Rb_tree_iterator<CBankFileAsyncData*>&,
    const _Rb_tree_iterator<CBankFileAsyncData*>&,
    const input_iterator_tag&);
}
