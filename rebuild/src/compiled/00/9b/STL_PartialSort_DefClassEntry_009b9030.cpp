#include <new>
#include "fable_definition_manager.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_vector_assign.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __push_heap(I first,D hole,D top,T value,C comp) {
 D parent=(hole-1)/2;
 while(hole>top&&comp(*(first+parent),value)){*(first+hole)=*(first+parent);hole=parent;parent=(hole-1)/2;}
 *(first+hole)=value;
}
}
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __adjust_heap(I first,D hole,D len,T value,C comp) {
 const D top=hole;D child=2*hole+2;
 while(child<len){if(comp(*(first+child),*(first+child-1)))--child;*(first+hole)=*(first+child);hole=child;child=2*(child+1);}
 if(child==len){*(first+hole)=*(first+child-1);hole=child-1;}
 __push_heap(first,hole,top,value,comp);
}
}
namespace _STL {
template<class I,class C,class T,class D> void FABLE_FASTCALL __make_heap(I first,I last,C comp,T*,D*) {
 if(last-first<2)return;
 D len=last-first;D parent=(len-2)/2;
 for(;;){__adjust_heap(first,parent,len,T(*(first+parent)),comp);if(parent==0)return;--parent;}
}
}
namespace _STL {
// Shared with the other heap unit; guarded so one translation unit can hold both.
#ifndef FABLE_SUBDEF_POP_HEAP_BODY
#define FABLE_SUBDEF_POP_HEAP_BODY
template<class I,class T,class C,class D> void FABLE_FASTCALL __pop_heap(I first,I last,I result,T value,C comp,D*) {
 *result=*first;
 __adjust_heap(first,D(0),D(last-first),value,comp);
}
#endif
template<class I,class C> void FABLE_FASTCALL sort_heap(I first,I last,C comp) {
 typedef typename iterator_traits<I>::value_type T;
 while(last-first>1){__pop_heap(first,last-1,last-1,T(*(last-1)),comp,(int*)0);--last;}
}
}namespace _STL {
// Shared with the other heap unit; guarded so one translation unit can hold both.
#ifndef FABLE_SUBDEF_POP_HEAP_BODY
#define FABLE_SUBDEF_POP_HEAP_BODY
template<class I,class T,class C,class D> void FABLE_FASTCALL __pop_heap(I first,I last,I result,T value,C comp,D*) {
 *result=*first;
 __adjust_heap(first,D(0),D(last-first),value,comp);
}
#endif
template<class I,class T,class C> void FABLE_FASTCALL __partial_sort(I first,I middle,I last,T*,C comp) {
 __make_heap(first,middle,comp,(T*)0,(int*)0);
 for(I i=middle;i<last;++i)if(comp(*i,*first))__pop_heap(first,middle,i,T(*i),comp,(int*)0);
 sort_heap(first,middle,comp);
}
}
typedef _STL::pair<unsigned long,CDefinitionManager::CDefClassInfo> Entry;
typedef CKeyPairCompareLess<unsigned long,CDefinitionManager::CDefClassInfo> Comp;
template void FABLE_FASTCALL _STL::__partial_sort(Entry*,Entry*,Entry*,Entry*,Comp);
