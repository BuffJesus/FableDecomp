#include "fable_threaded_file_object.h"
#include "fable_stlport_tree_bounds.h"
#include "fable_stlport_iterator_distance.h"
#include "fable_stlport_tree_erase.h"

typedef _STL::map<unsigned long,CThreadedFileManager::CFileObject*>::_Rep_type UidTree;
typedef _STL::_Rb_tree_node<_STL::pair<const unsigned long,CThreadedFileManager::CFileObject*> > UidNode;

namespace _STL {
template<class K,class V,class KeyOf,class Compare,class Alloc>
pair<typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::iterator,
     typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::iterator>
_Rb_tree<K,V,KeyOf,Compare,Alloc>::equal_range(const K& key)
{
    return pair<iterator,iterator>(lower_bound(key),upper_bound(key));
}
template<class K,class V,class KeyOf,class Compare,class Alloc>
typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::size_type
_Rb_tree<K,V,KeyOf,Compare,Alloc>::erase(const K& key)
{
    pair<iterator,iterator> range=equal_range(key);
    size_type count=static_cast<size_type>(distance(range.first,range.second));
    erase(range.first,range.second);
    return count;
}
template<class K,class V,class KeyOf,class Compare,class Alloc>
void _Rb_tree<K,V,KeyOf,Compare,Alloc>::erase(iterator first,iterator last)
{
    if (first==begin() && last==end())
        clear();
    else
        while (first!=last)
            erase(first++);
}
template<class I>
inline ptrdiff_t __cdecl __distance(const I& first,const I& last,const input_iterator_tag&)
{
    ptrdiff_t count=0;
    I current(first);
    while (current!=last) {
        ++current;
        ++count;
    }
    return count;
}
}
template<> void UidTree::_M_erase(UidNode* node)
{
    while (node) {
        _M_erase(static_cast<UidNode*>(node->_M_right));
        UidNode* left=static_cast<UidNode*>(node->_M_left);
        free(node);
        node=left;
    }
}
namespace _STL {
template UidTree::size_type UidTree::erase(const unsigned long&);
template pair<UidTree::iterator,UidTree::iterator> UidTree::equal_range(const unsigned long&);
template void UidTree::erase(UidTree::iterator,UidTree::iterator);
template ptrdiff_t __cdecl __distance<UidTree::iterator>(const UidTree::iterator&,const UidTree::iterator&,const input_iterator_tag&);
}
