// Behaviour of CResourceList::~CResourceList() 0x009FC5C0: the sentinel's
// destructor asks the list it is on (if any) to remove it, once, passing the
// sentinel itself; with no owning list nothing is called.
#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>

static int g_removeCalls;
static CResourceList* g_removeList;
static CResource* g_removed;

// Link scaffolding for the out-of-line members the vtables reference.
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
void CResourceList::RemoveResource(CResource& resource)
{
    ++g_removeCalls;
    g_removeList = this;
    g_removed = &resource;
}
bool CResourceList::UnloadNextResource(unsigned long) { return false; }
bool CResourceList::HandleFailedAllocation(unsigned long) { return false; }

int main()
{
    unsigned char storage[sizeof(CResourceList)];
    unsigned char owner[sizeof(CResourceList)];
    CResourceList* list = reinterpret_cast<CResourceList*>(storage);

    memset(storage, 0, sizeof(storage));
    list->CResourceList::~CResourceList();
    if (g_removeCalls != 0) { puts("unlisted head removed"); return 1; }

    memset(storage, 0, sizeof(storage));
    list->Head.ResourceList = reinterpret_cast<CResourceList*>(owner);
    list->CResourceList::~CResourceList();
    if (g_removeCalls != 1 || g_removeList != reinterpret_cast<CResourceList*>(owner) ||
        g_removed != &list->Head) { puts("remove"); return 1; }
    puts("FABLETLC_CRESOURCELIST_DTOR_009FC5C0_PASS");
    return 0;
}
