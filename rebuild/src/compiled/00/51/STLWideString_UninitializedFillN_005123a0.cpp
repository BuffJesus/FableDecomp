#include "fable_stlport_string.h"
namespace _STL {
 template<> wchar_t* FABLE_FASTCALL uninitialized_fill_n<wchar_t>(wchar_t* first,size_t count,const wchar_t& value)
 {
    for (; count > 0; --count, ++first)
        *first = value;
    return first;
 }
}
