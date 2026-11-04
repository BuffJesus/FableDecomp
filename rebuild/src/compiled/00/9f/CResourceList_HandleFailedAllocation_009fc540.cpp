// CResourceList::HandleFailedAllocation(unsigned long) @ 0x009FC540.
#include "fable_resource_list.h"

// A memory pool ran dry: unload the least recently used resource, whatever
// its size.
bool CResourceList::HandleFailedAllocation(unsigned long)
{
    return UnloadNextResource(0);
}
