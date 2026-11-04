#include "fable_game_text_bank.h"
unsigned long CBankFile::FindIndexBySymbol(const CCharString& symbol) const {
 // The shared STLport model currently exposes only its mutable iterator.
 // Lookup itself does not modify the map or its nodes.
 FableBankOwnedMap<CCharString,unsigned long>& indices=const_cast<FableBankOwnedMap<CCharString,unsigned long>&>(SymbolIndices);
 FableBankOwnedMap<CCharString,unsigned long>::iterator entry=indices.find(symbol);
 if(entry!=indices.end())return entry->second;
 return 0;
}

#include "fable_stlport_tree.h"
#include "fable_string.h"

namespace _STL
{
    // Walk down keeping the last node not below the key; that node is the
    // match unless the key is below it (or it is the header: end()).
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Base_ptr
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k)
    {
        _Base_ptr __header = this->_M_header._M_data;          // end()
        _Base_ptr __y = this->_M_header._M_data;               // Last node which is not less than __k.
        _Base_ptr __x = this->_M_header._M_data->_M_parent;    // Current node (the root).

        while (__x != 0)
            if (!_M_key_compare(static_cast<_Rb_tree_node<_Value>*>(__x)->_M_value_field.first, __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        if (__y == __header ||
            _M_key_compare(__k, static_cast<_Rb_tree_node<_Value>*>(__y)->_M_value_field.first))
            __y = __header;
        return __y;
    }

    template _Rb_tree<CCharString, pair<const CCharString, unsigned long>, _Select1st<pair<const CCharString, unsigned long> >,
                      less<CCharString>, allocator<pair<const CCharString, unsigned long> > >::_Base_ptr
    _Rb_tree<CCharString, pair<const CCharString, unsigned long>, _Select1st<pair<const CCharString, unsigned long> >,
             less<CCharString>, allocator<pair<const CCharString, unsigned long> > >::_M_find(const CCharString& __k);
}