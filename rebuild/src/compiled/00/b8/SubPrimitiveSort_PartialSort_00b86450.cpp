#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class T,class C> void FABLE_FASTCALL __partial_sort(I first,I middle,I last,T*,C compare){__make_heap(first,middle,compare,(T*)0,(ptrdiff_t*)0);for(I i=middle;i<last;++i)if(compare(*i,*first)){T value=*i;*i=*first;__adjust_heap(first,ptrdiff_t(0),ptrdiff_t(middle-first),value,compare);}sort_heap(first,middle,compare);}
template void FABLE_FASTCALL __partial_sort(CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,less<CEngineSubPrimitiveRenderer::CSortElement>);
}
