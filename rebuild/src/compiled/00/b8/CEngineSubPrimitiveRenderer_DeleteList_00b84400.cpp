#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_engine_subprimitive.h"
#include "fable_engine_primitive_manager.h"
void CEngineSubPrimitiveRenderer::DeleteList(CArray<CSortElement>& elements)
{
 unsigned long count=elements.size();
 for(unsigned long i=0;i<count;++i){
  CEngineSubPrimitiveBase* p=elements[i].SubPrimitive;
  do{CEngineSubPrimitiveBase* next=p->NextSubPrimitive;delete p;p=next;}while(p);
 }
 elements.erase(elements.begin(),elements.end());
}
