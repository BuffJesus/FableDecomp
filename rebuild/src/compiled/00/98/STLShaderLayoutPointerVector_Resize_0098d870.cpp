#include <new>
#include "fable_shader_constant_layout.h"
#include "fable_stlport_vector.h"
namespace _STL {
template<> void vector<const CVertexShaderConstantLayout*>::_M_fill_insert(const CVertexShaderConstantLayout**,size_t,const CVertexShaderConstantLayout* const&);
}
#include "fable_stlport_vector_impl.h"
template void _STL::vector<const CVertexShaderConstantLayout*>::resize(size_t,const CVertexShaderConstantLayout* const&);
