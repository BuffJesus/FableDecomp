#pragma optimize("s", on)
#include <new>
#include "fable_stlport_char_string_vector.h"
template<class I,class O> static __forceinline O StringUninitializedCopy(I first,I last,O result)
{O cur=result;for(;first!=last;++first,++cur)new(cur)CCharString(*first);return cur;}
template<class I,class O,class D> static __forceinline O CopyBackward(I first,I last,O result,const _STL::random_access_iterator_tag&, D*)
{for(D n=last-first;n>0;--n)*--result=*--last;return result;}
template<class I,class O> static __forceinline O StringCopyBackward(I first,I last,O result)
{return CopyBackward(first,last,result,_STL::random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class I,class T> static __forceinline void StringFill(I first,I last,const T& x)
{for(;first!=last;++first)*first=x;}
// The shared empty-tag shim has a user-provided constructor. This unit's
// retail STLport value-initializes its one-byte POD tag before passing it.
static __forceinline const _STL::__false_type& ValueInitializedTag(const _STL::__false_type& tag)
{ memset(const_cast<_STL::__false_type*>(&tag), 0, sizeof(tag)); return tag; }
namespace _STL {
    template <>
    void vector<CCharString>::_M_fill_insert(CCharString* __position, size_t __n, const CCharString& __x)
    {
        if (__n != 0)
        {
            if (size_t(this->_M_end_of_storage._M_data - this->_M_finish) >= __n)
            {
                CCharString __x_copy = __x;
                const size_t __elems_after = this->_M_finish - __position;
                CCharString* __old_finish = this->_M_finish;
                if (__elems_after > __n)
                {
                    StringUninitializedCopy(this->_M_finish - __n, this->_M_finish, this->_M_finish);
                    this->_M_finish += __n;
                    StringCopyBackward(__position, __old_finish - __n, __old_finish);
                    StringFill(__position, __position + __n, __x_copy);
                }
                else
                {
                    _STL::uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                    this->_M_finish += __n - __elems_after;
                    StringUninitializedCopy(__position, __old_finish, this->_M_finish);
                    this->_M_finish += __elems_after;
                    StringFill(__position, __old_finish, __x_copy);
                }
            }
            else
                _M_insert_overflow(__position, __x, ValueInitializedTag(__false_type()), __n);
        }
    }

}
template void _STL::vector<CCharString>::_M_fill_insert(CCharString*, size_t, const CCharString&);