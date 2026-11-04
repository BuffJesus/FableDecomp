// STLport vector<CVertexBufferWin32*>::_M_fill_insert, retail 0x009E3430 (reached
// from CFontManager's VertexBuffers.resize(8) 0x009E3580). Same body as the
// shader-layout pointer vector's 0x0098D000; fill is the out-of-line fastcall
// helper 0x009E2A80.
#include <new>
#define FABLE_STLP_FILL_CALL FABLE_FASTCALL
#include "fable_font_vertex_buffer.h"
#include "fable_stlport_vector_impl.h"
namespace _STL {
template<> __forceinline CVertexBufferWin32** __copy_backward_ptrs(
    CVertexBufferWin32** first,CVertexBufferWin32** last,
    CVertexBufferWin32** result,const __true_type&)
{
    // Retail shifts by the byte distance (no element-count round trip).
    const ptrdiff_t bytes=(const char*)last-(const char*)first;
    char* destination=(char*)result-bytes;
    if(bytes>0)memmove(destination,first,bytes);
    return (CVertexBufferWin32**)destination;
}
}
template void _STL::vector<CVertexBufferWin32*>::_M_fill_insert(CVertexBufferWin32**,size_t,CVertexBufferWin32* const&);
