// Behaviour of CResourceList::CResourceList() 0x009FC570: an empty list whose
// sentinel links to itself and is on no list, reference count 1, no memory cap,
// unload delay 1 and zeroed counters; HandleFailedAllocation dispatches through
// the new vtable.
#include "fable_resource_list.h"
#include <new>
#include <stdio.h>
#include <string.h>

// Link scaffolding for the out-of-line members the vtables reference.
static int g_unloadCalls;
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
void CResourceList::RemoveResource(CResource&) {}
bool CResourceList::UnloadNextResource(unsigned long) { ++g_unloadCalls; return true; }
bool CResourceList::HandleFailedAllocation(unsigned long size) { return UnloadNextResource(size); }
CResourceList::~CResourceList() {}

int main()
{
    unsigned char storage[sizeof(CResourceList)];
    memset(storage, 0xA5, sizeof(storage));
    CResourceList* list = new (storage) CResourceList();
    if (list->Head.PrevResource != &list->Head || list->Head.NextResource != &list->Head) { puts("links"); return 1; }
    if (list->Head.ResourceList != 0 || list->Head.ResourceSize != 0 || list->Head.LastUsedFrame != 0) { puts("head"); return 1; }
    if (list->Head.IVRefCount != 1) { puts("refcount"); return 1; }
    if (list->ResourceCount != 0 || list->AllocatedMemory != 0 || list->MaximumMemory != 0x7FFFFFFF ||
        list->CurrentFrame != 0 || list->DebugStatsFrame != 0 || list->UnloadDelay != 1 ||
        list->UnloadedThisFrame != 0) { puts("fields"); return 1; }
    CFailedAllocationHandler* handler = list;
    if (!handler->HandleFailedAllocation(16) || g_unloadCalls != 1) { puts("vtable"); return 1; }
    puts("FABLETLC_CRESOURCELIST_CTOR_009FC570_PASS");
    return 0;
}
