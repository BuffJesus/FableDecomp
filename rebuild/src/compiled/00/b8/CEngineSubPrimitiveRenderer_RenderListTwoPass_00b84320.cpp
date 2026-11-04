#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_engine_subprimitive.h"
#include "fable_engine_primitive_manager.h"
void CEngineSubPrimitiveRenderer::RenderListTwoPass(const CArray<CSortElement>& elements,ERenderPassType pre,ERenderPassType pass)
{
 unsigned long count=elements.size();
 for(unsigned long i=0;i<count;++i){
  CEngineSubPrimitiveBase* first=elements[i].SubPrimitive;
  CEngineSubPrimitiveBase* p=first;
  do{if(p->RenderFlags&4)p->Manager->RenderSubPrimitive(*p,pre);p=p->NextSubPrimitive;}while(p);
  p=first;
  do{p->Manager->RenderSubPrimitive(*p,pass);p=p->NextSubPrimitive;}while(p);
 }
}
