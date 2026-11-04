#pragma once
#include "fable_stlport_tree.h"
namespace _STL {
template<class K,class V,class KeyOf,class Compare,class Alloc>
inline void _Rb_tree<K,V,KeyOf,Compare,Alloc>::erase(iterator position) {
    _Rb_tree_node<V>* node=static_cast<_Rb_tree_node<V>*>(_Rb_global_inst::_Rebalance_for_erase(position._M_node,
        this->_M_header._M_data->_M_parent,this->_M_header._M_data->_M_left,this->_M_header._M_data->_M_right));
    _Destroy(&node->_M_value_field);
    if(node)free(node);
    --_M_node_count;
}
}
