// _STL::_Rb_tree<CCharString, pair<const CCharString, long>, ...>::insert_unique
// (const value_type&) (map<CCharString, long>) @ 0x00431606: STLport 4.6's
// unhinted insert (the hinted one, 0x00516AF0, falls back on it).
//
// Retail's unit is size-optimised (ebp frame, `this` homed at [ebp-4], the
// `pop ecx` after the cdecl _Rb_global<bool>::_M_decrement 0x004294E2) yet
// expands both key compares through less<CCharString>, CCharString::operator<
// and CBasicString<char>::operator< down to a CBasicString<char>::Compare
// (0x00411570) call. It does not see Compare's body (the key is reloaded on
// every pass), and it forces the small inline helpers: FABLE_UNIT_INLINE
// (rebuild_abi.h). _M_insert 0x004316CF stays a call.
#define FABLE_UNIT_INLINE __forceinline
#define FABLE_BASIC_STRING_DECLARATIONS_ONLY
#pragma optimize("s", on)
#include "fable_stlport_tree.h"
#include "fable_string.h"

namespace _STL
{
    // Walk down to the leaf where __v belongs; unless its key is already
    // there (the in-order predecessor of that slot is not below it), link a
    // new node in.
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const _Value& __v)
    {
        _Base_ptr __y = this->_M_header._M_data;
        _Base_ptr __x = this->_M_header._M_data->_M_parent;   // The root.
        bool __comp = true;
        while (__x != 0)
        {
            __y = __x;
            __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
            __x = __comp ? __x->_M_left : __x->_M_right;
        }
        iterator __j = iterator(__y);
        if (__comp)
            if (__j == iterator(this->_M_header._M_data->_M_left))   // begin()
                return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
            else
                --__j;
        if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
            return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
        return pair<iterator, bool>(__j, false);
    }

    template pair<_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >,
                           less<CCharString>, allocator<pair<const CCharString, long> > >::iterator, bool>
    _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >,
             less<CCharString>, allocator<pair<const CCharString, long> > >::insert_unique(const pair<const CCharString, long>& __v);
}