#include <new>
#include "fable_vertex_shader_input.h"
// Nested element assignment remains its own retail boundary (00A5E4D0).
namespace _STL {
template<> vector<CVertexStreamFormat::CDataElement>& vector<CVertexStreamFormat::CDataElement>::operator=(const vector<CVertexStreamFormat::CDataElement>&);
}
#include "fable_stlport_vector_assign.h"

template _STL::vector<CVertexShaderInput::CStreamInfo>& _STL::vector<CVertexShaderInput::CStreamInfo>::operator=(const _STL::vector<CVertexShaderInput::CStreamInfo>&);
