#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tempbuf.h"
namespace _STL {
template<class I,class T> void _Temporary_buffer<I,T>::_M_allocate_buffer(){_M_original_len=_M_len;_M_buffer=0;if(_M_len>ptrdiff_t(INT_MAX/sizeof(T)))_M_len=INT_MAX/sizeof(T);while(_M_len>0){_M_buffer=(T*)malloc(_M_len*sizeof(T));if(_M_buffer)break;_M_len/=2;}}
template void _Temporary_buffer<CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement>::_M_allocate_buffer();
}
