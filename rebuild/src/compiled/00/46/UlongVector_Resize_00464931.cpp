#pragma optimize("s",on)
#include <new>
#include "fable_stlport_vector.h"
namespace _STL {
    template<> unsigned long* vector<unsigned long>::erase(unsigned long*,unsigned long*);
    template<> void vector<unsigned long>::_M_fill_insert(unsigned long*,size_t,const unsigned long&);
}
#include "fable_stlport_vector_impl.h"
namespace _STL {
    template void vector<unsigned long>::resize(size_t,const unsigned long&);
}
