// Behaviour of CResource::CResource() 0x009FBEC0 (emitted out of line by the
// CResourceList ctor unit 0x009FC570, which also expands it): one reference,
// on no list, unlinked, unsized and never used.
#include "fable_resource_list.h"
#include <new>
#include <stdio.h>
#include <string.h>

// Link scaffolding for the out-of-line members the unit's vtables reference.
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
void CResourceList::RemoveResource(CResource&) {}
bool CResourceList::UnloadNextResource(unsigned long) { return true; }
bool CResourceList::HandleFailedAllocation(unsigned long size) { return UnloadNextResource(size); }
CResourceList::~CResourceList() {}

int main()
{
    unsigned char storage[sizeof(CResource)];
    memset(storage, 0xA5, sizeof(storage));
    CResource* r = new (storage) CResource();
    if (r->IVRefCount != 1) { puts("refcount"); return 1; }
    if (r->ResourceList != 0) { puts("list"); return 1; }
    if (r->PrevResource != 0 || r->NextResource != 0) { puts("links"); return 1; }
    if (r->ResourceSize != 0 || r->LastUsedFrame != 0) { puts("size/frame"); return 1; }
    if (*reinterpret_cast<void**>(storage) == (void*)0xA5A5A5A5) { puts("vtable"); return 1; }
    puts("FABLETLC_CRESOURCE_CTOR_009FBEC0_PASS");
    return 0;
}
