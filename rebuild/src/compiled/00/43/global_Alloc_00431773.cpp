// _STL::_Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_create_node
// (map<CCharString, long>) @ 0x00431773: STLport 4.6's node allocation,
// called twice by _M_insert 0x004316CF. malloc(0x18), then the value
// copy-constructed in place (the throw() placement new keeps its null test).
//
// Same size-optimised unit as insert_unique 0x00431606 (the `pop ecx` after
// malloc): FABLE_UNIT_INLINE (rebuild_abi.h) expands _Construct and pair's copy
// constructor down to the CCharString copy constructor 0x0099EC30, where the
// other /Os units call the pair copy (0x004297DB, from 0x0042B587).
#define FABLE_UNIT_INLINE __forceinline
#define FABLE_BASIC_STRING_DECLARATIONS_ONLY
#pragma optimize("s", on)
#include <new>
#include "fable_stlport_tree.h"
#include "fable_string.h"

namespace _STL
{
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    _Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_create_node(const _Value& __x)
    {
        _Rb_tree_node<_Value>* __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }

    template _Rb_tree_node<pair<const CCharString, long> >*
    _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >,
             less<CCharString>, allocator<pair<const CCharString, long> > >::_M_create_node(
                 const pair<const CCharString, long>& __x);
}
