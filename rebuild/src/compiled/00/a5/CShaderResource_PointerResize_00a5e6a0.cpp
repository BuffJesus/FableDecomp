#include <new>
#include "fable_vertex_shader.h"
#include "fable_stlport_vector.h"
// The separately compiled fill-insert provider is retail 00A5E3B0.
namespace _STL {
template<> void vector<CShaderBase*>::_M_fill_insert(CShaderBase**, size_t, CShaderBase* const&);
}
#include "fable_stlport_vector_impl.h"

template void _STL::vector<CShaderBase*>::resize(size_t, CShaderBase* const&);
