#include <new>
#include "fable_vertex_shader.h"
#include "fable_stlport_vector_impl.h"

namespace _STL {
// Retail inlines the POD backward-copy wrapper in 00A5E3B0.
template<> __forceinline CShaderBase** __copy_backward_ptrs(
    CShaderBase** first, CShaderBase** last, CShaderBase** result, const __true_type&)
{
    const ptrdiff_t count = last - first;
    if (count > 0)
        memmove(result - count, first, count * sizeof(CShaderBase*));
    return result - count;
}
}
template void _STL::vector<CShaderBase*>::_M_fill_insert(CShaderBase**, size_t, CShaderBase* const&);
