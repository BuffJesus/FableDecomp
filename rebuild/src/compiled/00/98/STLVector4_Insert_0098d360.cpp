#include <new>
#define FABLE_STLP_FILL_CALL FABLE_FASTCALL
#include "fable_4d_vector.h"
#include "fable_stlport_vector_impl.h"
namespace _STL {
    template<>
    void vector<C4DVector>::_M_fill_insert(C4DVector* __position, size_t __n, const C4DVector& __x)
    {
        if (__n != 0)
        {
            if (size_t(this->_M_end_of_storage._M_data - this->_M_finish) >= __n)
            {
                C4DVector __x_copy = __x;
                const size_t __elems_after = this->_M_finish - __position;
                C4DVector* __old_finish = this->_M_finish;
                if (__elems_after > __n)
                {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                    this->_M_finish += __n;
                    __copy_backward(__position, __old_finish - __n, __old_finish, random_access_iterator_tag(), (ptrdiff_t*)0);
                    fill(__position, __position + __n, __x_copy);
                }
                else
                {
                    __uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy, __false_type());
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
