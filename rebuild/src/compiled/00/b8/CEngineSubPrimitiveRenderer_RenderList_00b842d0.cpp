#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_engine_subprimitive.h"
#include "fable_engine_primitive_manager.h"
void CEngineSubPrimitiveRenderer::RenderList(const CArray<CSortElement>& elements,ERenderPassType pass)
{
 unsigned long count=elements.size();
 for(unsigned long i=0;i<count;++i){
  CEngineSubPrimitiveBase* p=elements[i].SubPrimitive;
  do{p->Manager->RenderSubPrimitive(*p,pass);p=p->NextSubPrimitive;}while(p);
 }
}
