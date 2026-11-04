#include "fable_console.h"
#include "fable_stlport_tree_bounds.h"

// CConsole::RegisteredConsoleInputs (map<CCharString, CCountedPointer<CConsoleInputBase> >)
// lookups, out of line in lib_console.cpp: _M_lower_bound 009EEC10 (SetGuessMode)
// and _M_find 009EED20 (GetRegisteredConsoleInput). Both expand CCharString's
// operator< and call CBasicString<char>::operator< 0x00429950.
namespace _STL {
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Base_ptr
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k)
    {
        _Base_ptr __y = this->_M_header._M_data;               // Last node which is not less than __k.
        _Base_ptr __x = this->_M_header._M_data->_M_parent;    // Current node (the root).

        while (__x != 0)
            if (!_M_key_compare(static_cast<_Rb_tree_node<_Value>*>(__x)->_M_value_field.first, __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        if (__y == this->_M_header._M_data ||
            _M_key_compare(__k, static_cast<_Rb_tree_node<_Value>*>(__y)->_M_value_field.first))
            __y = this->_M_header._M_data;
        return __y;
    }
}

typedef _STL::map<CCharString, CCountedPointer<CConsoleInputBase> >::_Rep_type CConsoleInputTree;
template CConsoleInputTree::_Link_type CConsoleInputTree::_M_lower_bound(const CCharString&) const;
template CConsoleInputTree::_Base_ptr CConsoleInputTree::_M_find(const CCharString&);
