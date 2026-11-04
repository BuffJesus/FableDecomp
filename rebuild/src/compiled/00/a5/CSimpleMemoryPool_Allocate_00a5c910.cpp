#include "fable_simple_memory_pool.h"
void* CSimpleMemoryPool::Allocate(unsigned long size)
{
 for(;;)
 {
  size=Align(size);
  CArea* previous=&Root;
  for(CArea* area=Root.NextFreeArea;area;area=area->NextFreeArea)
  {
   if(area->Size>=size)
   {
    previous->NextFreeArea=area->NextFreeArea;
    area->Status=STATUS_ALLOCATED;
    if(area->Size-size>16)
    {
     CreateArea((unsigned char*)area+16+size,area->Size-size);
     area->Size=size;
    }
    return area->GetDataPointer();
   }
   previous=area;
  }
  unsigned long capacity=FirstMemoryBlock?GrowthGranularity:Capacity;
  CreateNewMemoryBlock(capacity>size?capacity:size);
 }
}
