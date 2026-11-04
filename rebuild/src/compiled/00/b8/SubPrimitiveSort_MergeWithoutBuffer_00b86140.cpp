#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_merge.h"
namespace _STL {
template<class I,class D,class C> void FABLE_FASTCALL __merge_without_buffer(I first,I middle,I last,D len1,D len2,C compare){if(len1==0||len2==0)return;if(len1+len2==2){if(compare(*middle,*first)){typename iterator_traits<I>::value_type tmp=*first;*first=*middle;*middle=tmp;}return;}I firstCut=first,secondCut=middle;D len11=0,len22=0;if(len1>len2){len11=len1/2;firstCut+=len11;secondCut=__lower_bound(middle,last,*firstCut,compare,(D*)0);len22=secondCut-middle;}else{len22=len2/2;secondCut+=len22;firstCut=__upper_bound(first,middle,*secondCut,compare,(D*)0);len11=firstCut-first;}I newMiddle=__rotate(firstCut,middle,secondCut,(D*)0,random_access_iterator_tag());__merge_without_buffer(first,firstCut,newMiddle,len11,len22,compare);__merge_without_buffer(newMiddle,secondCut,last,len1-len11,len2-len22,compare);}
template void FABLE_FASTCALL __merge_without_buffer(CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,ptrdiff_t,ptrdiff_t,less<CEngineSubPrimitiveRenderer::CSortElement>);
}
