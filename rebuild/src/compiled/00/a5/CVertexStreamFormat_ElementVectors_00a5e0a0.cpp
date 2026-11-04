#include <new>
#include "fable_vertex_shader_input.h"
#include "fable_stlport_vector_assign.h"
#include "fable_stlport_vector_impl.h"

// Retail keeps the counted element-copy loop as a
// fastcall four-argument boundary; the generic wrapper is inlined here.
static __declspec(noinline) CVertexStreamFormat::CDataElement* __fastcall CopyVertexStreamElements(
    const CVertexStreamFormat::CDataElement* first, const CVertexStreamFormat::CDataElement* last,
    CVertexStreamFormat::CDataElement* result, const _STL::__false_type&)
{
    for (ptrdiff_t remaining=last-first; remaining>0; --remaining)
    {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
namespace _STL {
template<> __forceinline CVertexStreamFormat::CDataElement* __copy_ptrs(const CVertexStreamFormat::CDataElement* first, const CVertexStreamFormat::CDataElement* last, CVertexStreamFormat::CDataElement* result, const __false_type&)
{ return CopyVertexStreamElements(first,last,result,__false_type()); }
}

template _STL::vector<CVertexStreamFormat::CDataElement>& _STL::vector<CVertexStreamFormat::CDataElement>::operator=(const _STL::vector<CVertexStreamFormat::CDataElement>&);

template _STL::vector<CVertexStreamFormat::CDataElement>::vector(const _STL::vector<CVertexStreamFormat::CDataElement>&);
