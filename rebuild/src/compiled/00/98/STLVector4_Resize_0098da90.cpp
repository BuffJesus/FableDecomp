#include <new>
#include "fable_4d_vector.h"
#include "fable_stlport_vector.h"
namespace _STL {
template<> C4DVector* vector<C4DVector>::erase(C4DVector*,C4DVector*);
template<> void vector<C4DVector>::_M_fill_insert(C4DVector*,size_t,const C4DVector&);
}
#include "fable_stlport_vector_impl.h"
template void _STL::vector<C4DVector>::resize(size_t,const C4DVector&);
