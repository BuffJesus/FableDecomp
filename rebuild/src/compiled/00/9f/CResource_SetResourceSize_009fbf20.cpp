#include "fable_resource_list.h"
void CResource::SetResourceSize(unsigned long size) {
 if (ResourceList) { ResourceList->AllocatedMemory -= ResourceSize; ResourceList->AllocatedMemory += size; }
 ResourceSize = size;
}