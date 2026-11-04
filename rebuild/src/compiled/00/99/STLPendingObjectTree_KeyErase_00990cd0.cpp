#include "fable_threaded_file_object.h"
#include "fable_stlport_tree_bounds.h"
#include "fable_stlport_iterator_distance.h"
#include "fable_stlport_tree_erase.h"

typedef _STL::map<CThreadedFileManager::CPendingObjectSortKey,CThreadedFileManager::CFileObject*>::_Rep_type PendingTree;
typedef _STL::_Rb_tree_node<_STL::pair<const CThreadedFileManager::CPendingObjectSortKey,CThreadedFileManager::CFileObject*> > PendingNode;

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
// Share the identical recursive body emitted by manager destruction. Keep this
// specialization out of line at erase call sites, as in retail.
template<> __declspec(noinline) inline void PendingTree::_M_erase(PendingNode* node)
{
    while (node) {
        _M_erase(static_cast<PendingNode*>(node->_M_right));
        PendingNode* left=static_cast<PendingNode*>(node->_M_left);
        free(node);
        node=left;
    }
}
namespace _STL {
template PendingTree::size_type PendingTree::erase(const CThreadedFileManager::CPendingObjectSortKey&);
template PendingTree::_Link_type PendingTree::_M_lower_bound(const CThreadedFileManager::CPendingObjectSortKey&) const;
template PendingTree::_Link_type PendingTree::_M_upper_bound(const CThreadedFileManager::CPendingObjectSortKey&) const;
template void PendingTree::erase(PendingTree::iterator,PendingTree::iterator);
template ptrdiff_t __cdecl __distance<PendingTree::iterator>(const PendingTree::iterator&,const PendingTree::iterator&,const input_iterator_tag&);
}
