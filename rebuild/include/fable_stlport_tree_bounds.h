#pragma once

#include "fable_stlport_tree.h"

namespace _STL {
template<class K,class V,class KeyOf,class Compare,class Alloc>
typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::_Link_type
_Rb_tree<K,V,KeyOf,Compare,Alloc>::_M_lower_bound(const K& key) const
{
    _Link_type result=this->_M_header._M_data;
    _Link_type node=_M_root();
    while(node)
        if(!_M_key_compare(_S_key(node),key))
            result=node,node=_S_left(node);
        else
            node=_S_right(node);
    return result;
}
template<class K,class V,class KeyOf,class Compare,class Alloc>
typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::_Link_type
_Rb_tree<K,V,KeyOf,Compare,Alloc>::_M_upper_bound(const K& key) const
{
    _Link_type result=this->_M_header._M_data;
    _Link_type node=_M_root();
    while(node)
        if(_M_key_compare(key,_S_key(node)))
            result=node,node=_S_left(node);
        else
            node=_S_right(node);
    return result;
}
}
