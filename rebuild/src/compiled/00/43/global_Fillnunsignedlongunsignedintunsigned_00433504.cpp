#pragma optimize("s", on)
#include <new>
#include "fable_stlport_char_string_vector.h"
template<class T> T* FABLE_FASTCALL _STL::uninitialized_fill_n(T* first,size_t count,const T& value)
{ T* current=first; for(;count>0;--count,++current)new(current)T(value);return current; }
template CCharString* FABLE_FASTCALL _STL::uninitialized_fill_n<CCharString>(CCharString*,size_t,const CCharString&);
