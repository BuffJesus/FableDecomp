#include "fable_stlport_char_string_vector.h"
template <class T> void FABLE_FASTCALL _STL::_Destroy_range(T* first, T* last)
{
 for(;first!=last;++first) _STL::_Destroy(first);
}
template void FABLE_FASTCALL _STL::_Destroy_range<CCharString>(CCharString*, CCharString*);
