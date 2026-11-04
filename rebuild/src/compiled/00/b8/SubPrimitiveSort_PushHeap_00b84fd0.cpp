#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __push_heap(I first,D hole,D top,T value,C compare){D parent=(hole-1)/2;while(hole>top&&compare(*(first+parent),value)){*(first+hole)=*(first+parent);hole=parent;parent=(hole-1)/2;}*(first+hole)=value;}
template void FABLE_FASTCALL __push_heap(CEngineSubPrimitiveRenderer::CSortElement*,ptrdiff_t,ptrdiff_t,CEngineSubPrimitiveRenderer::CSortElement,less<CEngineSubPrimitiveRenderer::CSortElement>);
}
