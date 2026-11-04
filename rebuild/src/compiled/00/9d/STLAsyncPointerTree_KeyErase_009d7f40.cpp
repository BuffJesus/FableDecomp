#include "fable_game_text_bank.h"
#include "fable_stlport_iterator_distance.h"
namespace _STL {
template<class K, class V, class KeyOf, class Compare, class Alloc>
typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::size_type
_Rb_tree<K,V,KeyOf,Compare,Alloc>::erase(const K& key)
{
    pair<iterator,iterator> range = equal_range(key);
    size_type count = static_cast<size_type>(distance(range.first,range.second));
    erase(range.first,range.second);
    return count;
}
template _Rb_tree<CBankFileAsyncData*,CBankFileAsyncData*,
    _Identity<CBankFileAsyncData*>,less<CBankFileAsyncData*>,allocator<CBankFileAsyncData*> >::size_type
_Rb_tree<CBankFileAsyncData*,CBankFileAsyncData*,
    _Identity<CBankFileAsyncData*>,less<CBankFileAsyncData*>,allocator<CBankFileAsyncData*> >::erase(CBankFileAsyncData* const&);
}
