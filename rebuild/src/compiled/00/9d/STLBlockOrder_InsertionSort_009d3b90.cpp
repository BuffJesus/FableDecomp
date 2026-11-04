// Native STLport InsertionSort for CBankFile::InitBlockOrder entry indices at 0x009D3B90.
// Retail expands __linear_insert here, with copy_backward on the unsigned long
// indices taking STLport's trivial path (one memmove over the byte length); the
// shared headers keep the counted loop, so this unit specializes both.
#include "fable_game_text_bank.h"
#include "fable_stlport_linear_insert.h"
typedef CBankFile::CompareDataOffset Comp;
namespace _STL {
inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const ptrdiff_t _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : 0;
}
template<> inline unsigned long* copy_backward<unsigned long*, unsigned long*>(unsigned long* __first, unsigned long* __last, unsigned long* __result)
{
    return (unsigned long*)__copy_trivial_backward(__first, __last, __result);
}
template<> inline void FABLE_FASTCALL __linear_insert<unsigned long*, unsigned long, Comp>(unsigned long* __first, unsigned long* __last, unsigned long __val, Comp __comp)
{
    if (__comp(__val, *__first))
    {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
}
#include "fable_stlport_insertion_sort.h"
template void FABLE_FASTCALL _STL::__insertion_sort(unsigned long*,unsigned long*,Comp);
