#include "engine/CEngineSubPrimitiveRenderer.h"
void CEngineSubPrimitiveRenderer::SortSingleRadix(const CSortElement* input,CSortElement* output,unsigned long count,unsigned long radix)
{
 unsigned long histogram[1024]={0};
 unsigned long positions[1024];
 for(unsigned long i=0;i<count;++i)++histogram[(input[i].SortValue>>(radix*10))&1023];
 positions[0]=0;
 for(i=1;i<1024;++i)positions[i]=positions[i-1]+histogram[i-1];
 for(i=0;i<count;++i){unsigned long bucket=(input[i].SortValue>>(radix*10))&1023;output[positions[bucket]++]=input[i];}
}
