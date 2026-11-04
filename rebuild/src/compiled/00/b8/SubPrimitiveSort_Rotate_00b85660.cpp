#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_merge.h"
namespace _STL {
template<class I,class D> I FABLE_FASTCALL __rotate(I first,I middle,I last,D*,random_access_iterator_tag){typedef typename iterator_traits<I>::value_type T;D n=last-first,k=middle-first,l=n-k;I result=first+(last-middle);if(k==0)return last;if(k==l){swap_ranges(first,middle,middle);return result;}D a=n,b=k;while(b!=0){D c=a%b;a=b;b=c;}D d=a;for(D i=0;i<d;++i){T tmp=*first;I p=first;if(k<l){for(D j=0;j<l/d;++j){if(p>first+l){*p=*(p-l);p-=l;}*p=*(p+k);p+=k;}}else{for(D j=0;j<k/d-1;++j){if(p<last-k){*p=*(p+k);p+=k;}*p=*(p-l);p-=l;}}*p=tmp;++first;}return result;}
template CEngineSubPrimitiveRenderer::CSortElement* FABLE_FASTCALL __rotate(CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement*,ptrdiff_t*,random_access_iterator_tag);
}
