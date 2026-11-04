// _STL::basic_string<wchar_t>::append<const wchar_t*>(first, last,
// const forward_iterator_tag&) @ 0x0099C7E0 (STLport 4.6; CWideString::
// operator+= 0x0099B940 appends a literal to its data's string with it).
#include "fable_stlport_string.h"

template _STL::basic_string<wchar_t>& _STL::basic_string<wchar_t>::append<const wchar_t*>(
    const wchar_t* __first, const wchar_t* __last, const _STL::forward_iterator_tag&);