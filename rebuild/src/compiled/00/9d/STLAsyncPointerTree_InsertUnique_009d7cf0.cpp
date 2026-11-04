#include "fable_game_text_bank.h"

// Work-only ordinary generic STLport algorithm, canonical pointer instantiation.
// No forced inlining, private access scaffold, manual layout, or donor std names.
namespace _STL
{
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const _Value& __v)
    {
        _Base_ptr __y = this->_M_header._M_data;
        _Base_ptr __x = this->_M_header._M_data->_M_parent;
        bool __comp = true;
        while (__x != 0)
        {
            __y = __x;
            __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
            __x = __comp ? __x->_M_left : __x->_M_right;
        }
        iterator __j = iterator(__y);
        if (__comp)
            if (__j == iterator(this->_M_header._M_data->_M_left))
                return pair<iterator, bool>(_M_insert(__y, __y, __v), true);
            else
                --__j;
        if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
            return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
        return pair<iterator, bool>(__j, false);
    }

    template pair<_Rb_tree<CBankFileAsyncData*, CBankFileAsyncData*,
                          _Identity<CBankFileAsyncData*>, less<CBankFileAsyncData*>,
                          allocator<CBankFileAsyncData*> >::iterator, bool>
    _Rb_tree<CBankFileAsyncData*, CBankFileAsyncData*,
             _Identity<CBankFileAsyncData*>, less<CBankFileAsyncData*>,
             allocator<CBankFileAsyncData*> >::insert_unique(CBankFileAsyncData* const&);
}
