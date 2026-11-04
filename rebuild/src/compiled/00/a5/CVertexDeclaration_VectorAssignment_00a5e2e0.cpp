#include <new>
#include "fable_vertex_shader_input.h"
#include "fable_stlport_vector_assign.h"

// Retail keeps the counted declaration-copy loop at 00A5DE10 as a
// fastcall four-argument boundary; the generic wrapper is inlined here.
static __declspec(noinline) D3DVERTEXELEMENT9* __fastcall CopyVertexDeclarations(
    const D3DVERTEXELEMENT9* first, const D3DVERTEXELEMENT9* last,
    D3DVERTEXELEMENT9* result, const _STL::__false_type&)
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
template<> __forceinline D3DVERTEXELEMENT9* __copy_ptrs(const D3DVERTEXELEMENT9* first, const D3DVERTEXELEMENT9* last, D3DVERTEXELEMENT9* result, const __false_type&)
{ return CopyVertexDeclarations(first,last,result,__false_type()); }
}

template _STL::vector<D3DVERTEXELEMENT9>& _STL::vector<D3DVERTEXELEMENT9>::operator=(const _STL::vector<D3DVERTEXELEMENT9>&);
