#include <new>
#include "engine/CEngineTexturePalette.h"
#define FABLE_STLP_FILL_CALL FABLE_FASTCALL
#include "fable_stlport_algobase.h"
namespace _STL {
template<class T> inline T* FABLE_FASTCALL uninitialized_fill_n(T* first,size_t count,const T& value)
{
    T* cur=first;
    for(;count>0;--count,++cur)_Construct(cur,value);
    return cur;
}

template<> inline CEngineTexturePalette::CPaletteEntry* vector<CEngineTexturePalette::CPaletteEntry>::erase(CEngineTexturePalette::CPaletteEntry* first,CEngineTexturePalette::CPaletteEntry* last)
{
    CEngineTexturePalette::CPaletteEntry* result=__copy(last,_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);
    _Destroy(result,_M_finish);_M_finish=result;return first;
}
    template<>
    void vector<CEngineTexturePalette::CPaletteEntry>::_M_fill_insert(CEngineTexturePalette::CPaletteEntry* __position, size_t __n, const CEngineTexturePalette::CPaletteEntry& __x)
    {
        if (__n != 0)
        {
            if (size_t(this->_M_end_of_storage._M_data - this->_M_finish) >= __n)
            {
                CEngineTexturePalette::CPaletteEntry __x_copy = __x;
                const size_t __elems_after = this->_M_finish - __position;
                CEngineTexturePalette::CPaletteEntry* __old_finish = this->_M_finish;
                if (__elems_after > __n)
                {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                    this->_M_finish += __n;
                    __copy_backward(__position, __old_finish - __n, __old_finish, random_access_iterator_tag(), (ptrdiff_t*)0);
                    fill(__position, __position + __n, __x_copy);
                }
                else
                {
                    uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                    this->_M_finish += __elems_after;
                    fill(__position, __old_finish, __x_copy);
                }
            }
            else
                _M_insert_overflow(__position, __x, __false_type(), __n);
        }
    }

}
#include "fable_stlport_vector_impl.h"
template void _STL::vector<CEngineTexturePalette::CPaletteEntry>::resize(size_t);
