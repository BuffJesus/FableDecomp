// Scratch source-context reconstruction from untouched STLport 4.6.2 bounds.
// Only the async-pointer equal_range instantiation is emitted.
#include "fable_game_text_bank.h"
#include "fable_stlport_tree_bounds.h"
namespace _STL {
template<class K,class V,class KeyOf,class Compare,class Alloc>
pair<typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::iterator,
     typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::iterator>
_Rb_tree<K,V,KeyOf,Compare,Alloc>::equal_range(const K& key)
{
    return pair<iterator,iterator>(lower_bound(key),upper_bound(key));
}
template pair<_Rb_tree<CBankFileAsyncData*,CBankFileAsyncData*,
    _Identity<CBankFileAsyncData*>,less<CBankFileAsyncData*>,allocator<CBankFileAsyncData*> >::iterator,
    _Rb_tree<CBankFileAsyncData*,CBankFileAsyncData*,
    _Identity<CBankFileAsyncData*>,less<CBankFileAsyncData*>,allocator<CBankFileAsyncData*> >::iterator>
_Rb_tree<CBankFileAsyncData*,CBankFileAsyncData*,
    _Identity<CBankFileAsyncData*>,less<CBankFileAsyncData*>,allocator<CBankFileAsyncData*> >::equal_range(CBankFileAsyncData* const&);
}
