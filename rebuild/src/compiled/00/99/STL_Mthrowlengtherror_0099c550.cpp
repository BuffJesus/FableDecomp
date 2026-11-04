#include <stdio.h>
#include <stdlib.h>
#include "fable_stlport_string.h"

// STLport without exception support: _STLP_THROW_MSG(length_error, "basic_string")
// reports the error and aborts. Retail 0x0099C550 is the wide-string instantiation.
namespace _STL
{
    template <class _Tp, class _Alloc>
    void _String_base<_Tp, _Alloc>::_M_throw_length_error() const
    {
        puts("basic_string");
        abort();
    }

    template void _String_base<unsigned short, allocator<unsigned short> >::_M_throw_length_error() const;
}