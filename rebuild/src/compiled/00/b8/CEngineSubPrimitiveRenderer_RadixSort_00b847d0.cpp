#define FABLE_SUBPRIMITIVE_ERASE_DECLARATION_ONLY
#include "engine/CEngineSubPrimitiveRenderer.h"
#include <string.h>
#include "fable_stlport_vector_impl.h"
void CEngineSubPrimitiveRenderer::RadixSort(CArray<CSortElement>& elements)
{
 unsigned long count=elements.size();
 SortBuffer.resize(count);
 CSortElement* input=elements.GetAsCArray();
 CSortElement* output=SortBuffer.GetAsCArray();
 for(unsigned long radix=0;radix<3;++radix){
  unsigned long histogram[1024]={0};
  unsigned long positions[1024];
  for(unsigned long i=0;i<count;++i)++histogram[(input[i].SortValue>>(radix*10))&1023];
  positions[0]=0;
  for(i=1;i<1024;++i)positions[i]=positions[i-1]+histogram[i-1];
  for(i=0;i<count;++i){unsigned long bucket=(input[i].SortValue>>(radix*10))&1023;output[positions[bucket]++]=input[i];}
  _STL::swap(input,output);
 }
 if(input!=elements.GetAsCArray())memcpy(elements.GetAsCArray(),input,elements.size()*sizeof(CSortElement));
}
