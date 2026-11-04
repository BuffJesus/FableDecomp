#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __adjust_heap(I first,D hole,D len,T value,C compare){D top=hole;D second=2*hole+2;while(second<len){if(compare(*(first+second),*(first+second-1)))--second;*(first+hole)=*(first+second);hole=second;second=2*(second+1);}if(second==len){*(first+hole)=*(first+second-1);hole=second-1;}__push_heap(first,hole,top,value,compare);}
template void FABLE_FASTCALL __adjust_heap(CEngineSubPrimitiveRenderer::CSortElement*,ptrdiff_t,ptrdiff_t,CEngineSubPrimitiveRenderer::CSortElement,less<CEngineSubPrimitiveRenderer::CSortElement>);
}
