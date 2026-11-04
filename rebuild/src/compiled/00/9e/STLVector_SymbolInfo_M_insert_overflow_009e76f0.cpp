// _STL::vector<CSymbolMap::CSymbolInfo>::_M_insert_overflow
// (CSymbolInfo*, CSymbolInfo const&, __false_type const&, unsigned int, bool) @ 0x009E76F0
// (retail SymbolMap.cpp, beside the pair<CCharString, CSymbolInfo> copy
// 0x009E77E0). STLport's growth path in its own words: this instantiation
// ends in _M_clear() (kept out of line, 0x009E7250) and _M_set, where the
// pair copy's body in fable_stlport_vector_impl.h expands the clear itself.
#include "fable_symbol_map.h"
#include "fable_stlport_vector_impl.h"
#include <new>
typedef CSymbolMap::CSymbolInfo SI;

namespace _STL {
template <> void vector<SI>::_M_insert_overflow(SI* __position, const SI& __x,
    const __false_type&, size_t __fill_len, bool __atend)
{
    const size_t __old_size = size();
    const size_t __len = __old_size + (max)(__old_size, __fill_len);
    SI* __new_start = this->_M_end_of_storage.allocate(__len);
    SI* __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1)
    {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}
