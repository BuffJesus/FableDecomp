#pragma optimize("s", on)
#include "fable_stlport_char_string_vector.h"
#include "fable_stlport_algobase.h"
static __forceinline CCharString* CopyStringRange(CCharString* first, CCharString* last, CCharString* result)
{
 for(ptrdiff_t n=last-first;n>0;--n){*result=*first;++first;++result;}
 return result;
}
template <> CCharString* _STL::vector<CCharString>::erase(CCharString* first, CCharString* last)
{
 CCharString* finish = CopyStringRange(last, _M_finish, first);
 _STL::_Destroy_range(finish, _M_finish);
 _M_finish=finish;
 return first;
}

template CCharString* _STL::vector<CCharString>::erase(CCharString*, CCharString*);