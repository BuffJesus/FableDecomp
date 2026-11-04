#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class C,class T,class D> void FABLE_FASTCALL __make_heap(I first,I last,C compare,T*,D*){if(last-first<2)return;D len=last-first;D parent=(len-2)/2;for(;;){__adjust_heap(first,parent,len,T(*(first+parent)),compare);if(parent==0)return;--parent;}}
template void FABLE_FASTCALL __make_heap(CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,less<CEngineSubPrimitiveRenderer::CSortElement>,CEngineSubPrimitiveRenderer::CSortElement*,ptrdiff_t*);
}
