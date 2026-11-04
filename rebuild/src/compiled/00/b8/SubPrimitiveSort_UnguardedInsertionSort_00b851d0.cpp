#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_algo.h"
namespace _STL {
template<> void FABLE_FASTCALL __unguarded_insertion_sort_aux(CEngineSubPrimitiveRenderer::CSortElement* first,CEngineSubPrimitiveRenderer::CSortElement* last,CEngineSubPrimitiveRenderer::CSortElement*,less<CEngineSubPrimitiveRenderer::CSortElement> compare)
{
 for(CEngineSubPrimitiveRenderer::CSortElement* i=first;i!=last;++i){
  CEngineSubPrimitiveRenderer::CSortElement value=*i;
  CEngineSubPrimitiveRenderer::CSortElement* out=i;
  CEngineSubPrimitiveRenderer::CSortElement* next=i-1;
  while(compare(value,*next)){*out=*next;out=next;--next;}
  *out=value;
 }
}
}
