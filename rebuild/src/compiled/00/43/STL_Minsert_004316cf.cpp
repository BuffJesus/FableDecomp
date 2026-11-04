// _STL::_Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_insert
// (x, y, v, w) (map<CCharString, long>) @ 0x004316CF: STLport 4.6's link step,
// called by both insert_unique overloads (0x00431606, 0x00516AF0).
//
// Same size-optimised unit as insert_unique 0x00431606 (`cmp [ebp+18h], 0`,
// `and [esi+8], 0`, an ebp frame): the key compare expands down to a
// CBasicString<char>::Compare call through FABLE_UNIT_INLINE (rebuild_abi.h),
// and _M_create_node 0x00431773 and _Rb_global<bool>::_Rebalance 0x0042971D
// stay calls.
#define FABLE_UNIT_INLINE __forceinline
#define FABLE_BASIC_STRING_DECLARATIONS_ONLY
#pragma optimize("s", on)
#include "fable_stlport_tree.h"
#include "fable_string.h"

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

    template _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >,
                      less<CCharString>, allocator<pair<const CCharString, long> > >::iterator
    _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >,
             less<CCharString>, allocator<pair<const CCharString, long> > >::_M_insert(_Rb_tree_node_base* __x,
                 _Rb_tree_node_base* __y, const pair<const CCharString, long>& __v, _Rb_tree_node_base* __w);
}