#include <new>
#include "fable_render2d_vertex.h"
#include "fable_stlport_algobase.h"
namespace _STL {
template<> inline CTVertexRHWColSpecTex1* vector<CTVertexRHWColSpecTex1>::erase(CTVertexRHWColSpecTex1* first,CTVertexRHWColSpecTex1* last)
{
 CTVertexRHWColSpecTex1* finish=__copy(last,this->_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);
 _Destroy(finish,this->_M_finish);this->_M_finish=finish;return first;
}
}

namespace _STL { template CTVertexRHWColSpecTex1* vector<CTVertexRHWColSpecTex1>::erase(CTVertexRHWColSpecTex1*,CTVertexRHWColSpecTex1*); }
