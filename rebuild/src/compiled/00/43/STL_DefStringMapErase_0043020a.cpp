#pragma optimize("s", on)
#include "fable_def_string.h"
#include <stdlib.h>

namespace _STL {
template<class K, class V, class KeyOf, class Compare, class Alloc>
void _Rb_tree<K,V,KeyOf,Compare,Alloc>::_M_erase(_Rb_tree_node<V>* node)
{
    while (node)
    {
        _M_erase(static_cast<_Rb_tree_node<V>*>(node->_M_right));
        _Rb_tree_node<V>* left = static_cast<_Rb_tree_node<V>*>(node->_M_left);
        node->_M_value_field.~V();
        free(node);
        node = left;
    }
}
}
typedef _STL::pair<const CCharString,long> DefStringMapEntry;
typedef _STL::_Rb_tree<CCharString,DefStringMapEntry,_STL::_Select1st<DefStringMapEntry>,_STL::less<CCharString> > DefStringMapTree;
template void DefStringMapTree::_M_erase(_STL::_Rb_tree_node<DefStringMapEntry>*);
