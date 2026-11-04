#include "fable_game_text_bank.h"
#include "fable_stlport_tree_erase.h"

namespace _STL {
template<class K, class V, class KeyOf, class Compare, class Alloc>
void _Rb_tree<K, V, KeyOf, Compare, Alloc>::erase(iterator first, iterator last)
{
    if (first == begin() && last == end())
        clear();
    else
        while (first != last)
            erase(first++);
}

template void _Rb_tree<CBankFileAsyncData*, CBankFileAsyncData*,
    _Identity<CBankFileAsyncData*>, less<CBankFileAsyncData*>,
    allocator<CBankFileAsyncData*> >::erase(iterator, iterator);
}
