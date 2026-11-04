#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_tempbuf.h"
#include "fable_stlport_algo.h"
namespace _STL {
template<class I,class D,class C> void FABLE_FASTCALL __merge_without_buffer(I,I,I,D,D,C);
template<class I,class C> void FABLE_FASTCALL __inplace_stable_sort(I first,I last,C compare){if(last-first<15){__insertion_sort(first,last,compare);return;}I middle=first+(last-first)/2;__inplace_stable_sort(first,middle,compare);__inplace_stable_sort(middle,last,compare);__merge_without_buffer(first,middle,last,middle-first,last-middle,compare);}
template void FABLE_FASTCALL __inplace_stable_sort(CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,less<CEngineSubPrimitiveRenderer::CSortElement>);
}
