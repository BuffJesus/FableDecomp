#pragma once

// Retail STLport 4.x vector::operator= (stl/_vector.c), opt-in like
// fable_stlport_vector_impl.h. <CWideString>: 0x00433DF0 (CDefinitionManager::
// SetSymbolPathList / SetCompilePathList; a FABLE_UNIT_INLINE unit that also
// needs fable_stlport_uninitialized.h). <unsigned long>: 0x0045BC09, from a
// size-optimised unit that keeps _M_allocate_and_copy 0x0045BCF2, _M_clear
// 0x0045BCE4 and the POD __uninitialized_copy 0x0045BCC1 out of line.

#include "fable_stlport_vector.h"
#include "fable_stlport_algobase.h"

namespace _STL
{
    // STLport __copy_ptrs for trivially assignable elements: one memmove.
    template <class _Tp>
    inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
    {
        return (_Tp*)__copy_trivial(__first, __last, __result);
    }

    // STLport __copy_ptrs for class elements: the counted assignment loop.
    template <class _Tp>
    FABLE_UNIT_INLINE _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
    {
        return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
    }

    // Reallocate when the source does not fit; otherwise assign over the
    // existing elements, then construct the rest (or destroy the surplus).
    template <class _Tp, class _Alloc>
    vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
    {
        typedef typename __type_traits<_Tp>::is_POD_type _TrivialAss;
        typedef typename __type_traits<_Tp>::is_POD_type _IsPODType;
        if (&__x != this)
        {
            const size_t __xlen = __x.size();
            if (__xlen > capacity())
            {
                _Tp* __tmp = _M_allocate_and_copy(__xlen, (const _Tp*)__x._M_start + 0, (const _Tp*)__x._M_finish + 0);
                _M_clear();
                this->_M_start = __tmp;
                this->_M_end_of_storage._M_data = this->_M_start + __xlen;
            }
            else if (size() >= __xlen)
            {
                _Tp* __i = __copy_ptrs((const _Tp*)__x._M_start + 0, (const _Tp*)__x._M_finish + 0,
                                       (_Tp*)this->_M_start, _TrivialAss());
                _Destroy(__i, this->_M_finish);
            }
            else
            {
                __copy_ptrs((const _Tp*)__x._M_start, (const _Tp*)__x._M_start + size(), (_Tp*)this->_M_start,
                            _TrivialAss());
                __uninitialized_copy((const _Tp*)__x._M_start + size(), (const _Tp*)__x._M_finish + 0,
                                     this->_M_finish, _IsPODType());
            }
            this->_M_finish = this->_M_start + __xlen;
        }
        return *this;
    }
}
