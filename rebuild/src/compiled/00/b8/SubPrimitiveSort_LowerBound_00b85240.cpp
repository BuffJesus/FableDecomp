#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_merge.h"
namespace _STL {
template<class I,class T,class C,class D> I FABLE_FASTCALL __lower_bound(I first,I last,const T& value,C compare,D*){D len=last-first;while(len>0){D half=len>>1;I middle=first+half;if(compare(*middle,value)){first=middle+1;len=len-half-1;}else len=half;}return first;}
template CEngineSubPrimitiveRenderer::CSortElement* FABLE_FASTCALL __lower_bound(CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,const CEngineSubPrimitiveRenderer::CSortElement&,less<CEngineSubPrimitiveRenderer::CSortElement>,ptrdiff_t*);
}
