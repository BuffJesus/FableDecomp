#include <new>
#include "fable_stlport_tree.h"
class CBankFileAsyncData;

// Use the already reconstructed generic rebalance provider as a call.
template<> void __cdecl _STL::_Rb_global<bool>::_Rebalance(
    _STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*&);

namespace _STL {
// Ordinary STLport allocation + copy-construction context. No pointer layout
// stores substitute for allocator/proxy/_Construct semantics.
template<class K,class V,class KV,class C,class A>
_Rb_tree_node<V>* _Rb_tree<K,V,KV,C,A>::_M_create_node(const V& value) {
    _Rb_tree_node<V>* node = this->_M_header.allocate(1);
    _Construct(&node->_M_value_field, value);
    return node;
}

template<class K,class V,class KV,class C,class A>
typename _Rb_tree<K,V,KV,C,A>::iterator
_Rb_tree<K,V,KV,C,A>::_M_insert(_Base_ptr x,_Base_ptr y,const V& value,_Base_ptr w) {
    _Base_ptr node;
    if (y == this->_M_header._M_data ||
        (w == 0 && (x != 0 || _M_key_compare(KV()(value), _S_key(y))))) {
        node = _M_create_node(value);
        y->_M_left = node;
        if (y == this->_M_header._M_data) {
            this->_M_header._M_data->_M_parent = node;
            _M_rightmost() = node;
        } else if (y == _M_leftmost()) {
            _M_leftmost() = node;
        }
    } else {
        node = _M_create_node(value);
        y->_M_right = node;
        if (y == _M_rightmost())
            _M_rightmost() = node;
    }
    node->_M_parent = y;
    node->_M_left = 0;
    node->_M_right = 0;
    _Rb_global_inst::_Rebalance(node, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(node);
}

template _Rb_tree<CBankFileAsyncData*,CBankFileAsyncData*,
    _Identity<CBankFileAsyncData*>,less<CBankFileAsyncData*>,
    allocator<CBankFileAsyncData*> >::iterator
_Rb_tree<CBankFileAsyncData*,CBankFileAsyncData*,
    _Identity<CBankFileAsyncData*>,less<CBankFileAsyncData*>,
    allocator<CBankFileAsyncData*> >::_M_insert(
        _Rb_tree_node_base*,_Rb_tree_node_base*,CBankFileAsyncData* const&,
        _Rb_tree_node_base*);
}
