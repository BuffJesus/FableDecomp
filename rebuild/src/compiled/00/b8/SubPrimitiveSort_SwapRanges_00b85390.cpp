#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_merge.h"
namespace _STL {
template<class I,class J> J FABLE_FASTCALL swap_ranges(I first,I last,J out){for(;first!=last;++first,++out){typename iterator_traits<I>::value_type tmp=*first;*first=*out;*out=tmp;}return out;}
template CEngineSubPrimitiveRenderer::CSortElement* FABLE_FASTCALL swap_ranges(CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*);
}
