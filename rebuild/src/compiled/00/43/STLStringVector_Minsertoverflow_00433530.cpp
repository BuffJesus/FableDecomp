#include <new>
#include "fable_stlport_char_string_vector.h"
template<class I,class O> static __forceinline O StringUninitializedCopy(I first,I last,O result)
{O cur=result;for(;first!=last;++first,++cur)new(cur)CCharString(*first);return cur;}
namespace _STL {
    template <>
    void vector<CCharString>::_M_insert_overflow(CCharString* __position, const CCharString& __x,
                                                 const __false_type& /*DO NOT USE!!*/, size_t __fill_len,
                                                 bool __atend)
    {
        const size_t __old_size = size();
        const size_t __len = __old_size + (max)(__old_size, __fill_len);

        CCharString* __new_start = this->_M_end_of_storage.allocate(__len);
        CCharString* __new_finish = StringUninitializedCopy(this->_M_start, __position, __new_start);
        if (__fill_len == 1)
        {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = StringUninitializedCopy(__position, this->_M_finish, __new_finish);

        __destroy_aux(this->_M_start, this->_M_finish, __false_type());
        if (this->_M_start != 0)
            this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
        this->_M_start = __new_start;
        this->_M_finish = __new_finish;
        this->_M_end_of_storage._M_data = __new_start + __len;
    }

}