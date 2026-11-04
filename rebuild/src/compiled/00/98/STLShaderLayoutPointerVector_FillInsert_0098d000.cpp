#include <new>
#define FABLE_STLP_FILL_CALL FABLE_FASTCALL
#include "fable_shader_constant_layout.h"
#include "fable_stlport_vector_impl.h"
namespace _STL {
template<> __forceinline const CVertexShaderConstantLayout** __copy_backward_ptrs(
    const CVertexShaderConstantLayout** first,const CVertexShaderConstantLayout** last,
    const CVertexShaderConstantLayout** result,const __true_type&)
{
    // Retail shifts by the byte distance (no element-count round trip).
    const ptrdiff_t bytes=(const char*)last-(const char*)first;
    char* destination=(char*)result-bytes;
    if(bytes>0)memmove(destination,first,bytes);
    return (const CVertexShaderConstantLayout**)destination;
}
}
template void _STL::vector<const CVertexShaderConstantLayout*>::_M_fill_insert(const CVertexShaderConstantLayout**,size_t,const CVertexShaderConstantLayout* const&);
