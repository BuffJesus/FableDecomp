#include <new>
#include "fable_render2d_vertex.h"
#include "fable_stlport_uninitialized.h"

namespace _STL {
template<> void vector<CTVertexRHWColSpecTex1>::_M_insert_overflow(
    CTVertexRHWColSpecTex1* position, const CTVertexRHWColSpecTex1& value,
    const __false_type&, size_t fillLength, bool atEnd)
{
    const size_t oldSize = size();
    const size_t length = oldSize + (max)(oldSize, fillLength);
    CTVertexRHWColSpecTex1* start = _M_end_of_storage.allocate(length);
    CTVertexRHWColSpecTex1* finish = __uninitialized_copy(_M_start, position, start, __false_type());
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
