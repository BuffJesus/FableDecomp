#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_algo.h"
#include "fable_stlport_tree.h"
namespace _STL {
template<class I> void FABLE_FASTCALL sort(I first,I last)
{
 if(first!=last){
 __introsort_loop(first,last,__value_type_of(first),__lg(last-first)*2,less<typename iterator_traits<I>::value_type>());
 __final_insertion_sort(first,last,less<typename iterator_traits<I>::value_type>());
 }
}
template void FABLE_FASTCALL sort<CEngineSubPrimitiveRenderer::CSortElement*>(CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*);
}
