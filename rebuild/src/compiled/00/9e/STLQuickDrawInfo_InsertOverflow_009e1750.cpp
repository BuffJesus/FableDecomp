#include <new>
#include "fable_quick_draw_info.h"
#include "fable_stlport_uninitialized.h"

namespace _STL {
template<> void vector<CQuickDrawTriInfo>::_M_insert_overflow(
    CQuickDrawTriInfo* position, const CQuickDrawTriInfo& value,
    const __false_type&, size_t fillLength, bool atEnd)
{
    const size_t oldSize = size();
    const size_t length = oldSize + (max)(oldSize, fillLength);
    CQuickDrawTriInfo* start = _M_end_of_storage.allocate(length);
    CQuickDrawTriInfo* finish = __uninitialized_copy(_M_start, position, start, __false_type());
    if (fillLength == 1) {
        _Construct(finish, value);
        ++finish;
    } else {
        finish = __uninitialized_fill_n(finish, fillLength, value, __false_type());
    }
    if (!atEnd)
        finish = __uninitialized_copy(position, _M_finish, finish, __false_type());
    _M_clear();
    _M_set(start, finish, start + length);
}
}
