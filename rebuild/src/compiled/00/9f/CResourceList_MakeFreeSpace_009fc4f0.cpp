#include "fable_resource_list.h"
bool CResourceList::MakeFreeSpace(unsigned long size) { ShrinkResources(size); if (AllocatedMemory + size > (unsigned long)MaximumMemory) return false; return true; }