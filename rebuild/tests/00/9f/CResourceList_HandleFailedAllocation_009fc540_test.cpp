// Behaviour of CResourceList::HandleFailedAllocation(unsigned long) 0x009FC540:
// whatever size failed, it unloads the next resource with no size target
// (UnloadNextResource(0) on the same list) and returns that result.
#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>

static int g_calls;
static CResourceList* g_this;
static unsigned long g_size;
static bool g_result;

// Link scaffolding for the out-of-line members the vtables reference.
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
void CResourceList::RemoveResource(CResource&) {}
bool CResourceList::UnloadNextResource(unsigned long size)
{
    ++g_calls;
    g_this = this;
    g_size = size;
    return g_result;
}

int main()
{
    unsigned char storage[sizeof(CResourceList)];
    memset(storage, 0, sizeof(storage));
    CResourceList* list = reinterpret_cast<CResourceList*>(storage);
    g_result = true;
    if (!list->CResourceList::HandleFailedAllocation(0x1234) || g_calls != 1 || g_this != list || g_size != 0) { puts("true"); return 1; }
    g_result = false;
    if (list->CResourceList::HandleFailedAllocation(7) || g_calls != 2 || g_size != 0) { puts("false"); return 1; }
    puts("FABLETLC_CRESOURCELIST_HANDLEFAILEDALLOCATION_009FC540_PASS");
    return 0;
}
