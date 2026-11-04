// CResourceList::SetMaximumMemory(unsigned long) @ 0x009FC520
// (Ego_r/FableWin ?SetMaximumMemory@CResourceList@@QAEXK@Z).
#include "fable_resource_list.h"

void CResourceList::SetMaximumMemory(unsigned long maximumMemory)
{
    MaximumMemory = maximumMemory;
    ShrinkResources(0);
}
