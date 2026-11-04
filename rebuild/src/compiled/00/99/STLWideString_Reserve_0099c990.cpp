#include "fable_stlport_string.h"
namespace _STL {
 template<class C,class T,class A>
 void basic_string<C,T,A>::reserve(size_t count)
 {
    if (count > this->max_size())
        this->_M_throw_length_error();
    const size_t old_size = size();
    const size_t length = (max)(count, old_size) + 1;
    C* first = this->_M_end_of_storage.allocate(length);
    C* finish = static_cast<C*>(__copy_trivial(this->_M_start, this->_M_finish, first));
    *finish = C();
    this->_M_deallocate_block();
    this->_M_start = first;
    this->_M_finish = finish;
    this->_M_end_of_storage._M_data = first + length;
 }
 template void basic_string<wchar_t>::reserve(size_t count);
}
