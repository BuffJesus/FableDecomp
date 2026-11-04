#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class C> void FABLE_FASTCALL sort_heap(I first,I last,C compare){typedef typename iterator_traits<I>::value_type T;while(last-first>1){T value=*(last-1);*(last-1)=*first;__adjust_heap(first,ptrdiff_t(0),ptrdiff_t((last-1)-first),value,compare);--last;}}
template void FABLE_FASTCALL sort_heap(CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,less<CEngineSubPrimitiveRenderer::CSortElement>);
}
