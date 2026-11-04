#include <new>
#include "fable_string.h"
#include "fable_stlport_tree.h"
// Use the existing retail-sized rebalance provider, not a second /O2 COMDAT.
template<> void __cdecl _STL::_Rb_global<bool>::_Rebalance(_STL::_Rb_tree_node_base*,_STL::_Rb_tree_node_base*&);
namespace _STL {
template<class K,class V,class KV,class C,class A>
_Rb_tree_node<V>* _Rb_tree<K,V,KV,C,A>::_M_create_node(const V& value) {
 _Rb_tree_node<V>* node=this->_M_header.allocate(1);_Construct(&node->_M_value_field,value);return node;
}
}
namespace _STL
{
    // Hang a new node for __v under __y (left when __y is the header, when __x
    // is set, or when the key sorts first, unless __w is set), keep the
    // leftmost/rightmost links, rebalance and count it.
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x, _Base_ptr __y, const _Value& __v,
                                                                     _Base_ptr __w)
    {
        _Base_ptr __z;

        if (__y == this->_M_header._M_data ||
            (__w == 0 &&                  // If __w is set the rest is false.
             (__x != 0 ||                 // If __x is set the rest is true.
              _M_key_compare(_KeyOfValue()(__v), _S_key(__y)))))
        {
            __z = _M_create_node(__v);
            __y->_M_left = __z;           // Also makes _M_leftmost() = __z when __y is the header.
            if (__y == this->_M_header._M_data)
            {
                this->_M_header._M_data->_M_parent = __z;   // The root.
                _M_rightmost() = __z;
            }
            else if (__y == _M_leftmost())
                _M_leftmost() = __z;      // Keep _M_leftmost() at the minimum.
        }
        else
        {
            __z = _M_create_node(__v);
            __y->_M_right = __z;
            if (__y == _M_rightmost())
                _M_rightmost() = __z;     // Keep _M_rightmost() at the maximum.
        }
        __z->_M_parent = __y;
        __z->_M_left = 0;
        __z->_M_right = 0;
        _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
        ++_M_node_count;
        return iterator(__z);
    }

    template _Rb_tree<CCharString,CCharString,_Identity<CCharString>,less<CCharString>,allocator<CCharString> >::iterator
    _Rb_tree<CCharString,CCharString,_Identity<CCharString>,less<CCharString>,allocator<CCharString> >::_M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const CCharString&,_Rb_tree_node_base*);
}
