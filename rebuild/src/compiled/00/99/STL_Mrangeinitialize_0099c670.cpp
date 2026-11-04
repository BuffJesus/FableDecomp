// _STL::basic_string<wchar_t>::_M_range_initialize<const wchar_t*> @ 0x0099C670.
// STLport's forward-iterator range initialiser, out of line in retail; the
// C-string ctors inlined into CWideString::AllocStringData 0x0099B3C0 and
// MakeOwnCopyOfData 0x0099B560 call it with a random_access_iterator_tag.
#include "fable_stlport_string.h"

template void _STL::basic_string<wchar_t>::_M_range_initialize<const wchar_t*>(
    const wchar_t* __f, const wchar_t* __l, const _STL::forward_iterator_tag&);