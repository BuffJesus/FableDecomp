#include <new>
#include "fable_vertex_shader_input.h"
namespace _STL {
template<> void vector<CVertexShaderInput::CStreamInfo>::_M_insert_overflow(CVertexShaderInput::CStreamInfo*,const CVertexShaderInput::CStreamInfo&,const __false_type&,size_t,bool);
template<> vector<CVertexStreamFormat::CDataElement>::vector(const vector<CVertexStreamFormat::CDataElement>&);
}
#include "fable_stlport_vector_impl.h"
void CVertexShaderInput::AddStream(unsigned long index,const CVertexStreamFormat& format,bool generate)
{
 DataStreams.push_back(CStreamInfo(index,format));
 if(generate)GenerateD3DDeclaration();
}
