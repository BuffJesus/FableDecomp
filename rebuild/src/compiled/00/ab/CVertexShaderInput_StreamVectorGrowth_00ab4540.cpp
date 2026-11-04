#include <new>
#include "fable_vertex_shader_input.h"
namespace _STL {
template<> vector<CVertexStreamFormat::CDataElement>::vector(const vector<CVertexStreamFormat::CDataElement>&);
}
#include "fable_stlport_vector_impl.h"
template void _STL::vector<CVertexShaderInput::CStreamInfo>::_M_insert_overflow(CVertexShaderInput::CStreamInfo*,const CVertexShaderInput::CStreamInfo&,const _STL::__false_type&,size_t,bool);
