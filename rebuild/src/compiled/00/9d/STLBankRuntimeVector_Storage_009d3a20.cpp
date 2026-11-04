#define FABLE_STLP_FILL_CALL __fastcall
#include <new>
#include "fable_game_text_bank.h"
#include "fable_stlport_uninitialized.h"
#include "fable_stlport_vector_impl.h"
namespace _STL {
template<class T> inline T* FABLE_FASTCALL uninitialized_fill_n(T* first,size_t n,const T& x) {
T* cur=first;for(;n>0;--n,++cur)_Construct(&*cur,x);return cur;
}
    template<>
    void vector<CBankFile::CRuntimeData>::_M_fill_insert(CBankFile::CRuntimeData* __position, size_t __n, const CBankFile::CRuntimeData& __x)
    {
        if (__n != 0)
        {
            if (size_t(this->_M_end_of_storage._M_data - this->_M_finish) >= __n)
            {
                CBankFile::CRuntimeData __x_copy = __x;
                const size_t __elems_after = this->_M_finish - __position;
                CBankFile::CRuntimeData* __old_finish = this->_M_finish;
                if (__elems_after > __n)
                {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                    this->_M_finish += __n;
                    copy_backward(__position, __old_finish - __n, __old_finish);
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
