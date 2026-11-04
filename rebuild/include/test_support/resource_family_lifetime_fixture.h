#include "fable_game_text_bank.h"
#include <stdio.h>
#include <stdlib.h>
static unsigned int allocations,frees;
void* operator new(size_t size) { ++allocations;return malloc(size); }
void operator delete(void* data) { ++frees;free(data); }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
bool CResourceList::HandleFailedAllocation(unsigned long) { return false; }
// MSVC deleting-destructor ABI view: slot 0 takes a flags word and returns this.
struct DeletingView { virtual void* Destroy(unsigned int flags); };
#define CHECK(x) do { if(!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    CResourceList owner;
    CResource* resource=new CResource;
    CHECK(resource->IVRefCount==1 && !resource->ResourceList && !resource->PrevResource && !resource->NextResource && !resource->ResourceSize && !resource->LastUsedFrame);
    resource->ResourceSize=42;owner.AddResource(*resource);
    CHECK(owner.ResourceCount==1 && owner.AllocatedMemory==42);
    CHECK(resource->Release()==0);
    CHECK(owner.ResourceCount==0 && owner.AllocatedMemory==0 && allocations==1 && frees==1);
    CResource* retained=new CResource;
    retained->ResourceSize=17;owner.AddResource(*retained);
    void* returned=reinterpret_cast<DeletingView*>(retained)->Destroy(0);
    CHECK(returned==retained && owner.ResourceCount==0 && owner.AllocatedMemory==0 && allocations==2 && frees==1);
    ::operator delete(returned);
    CHECK(frees==2);
    CResourceBank* bank=new CResourceBank;
    bank->ResourceList.Head.ResourceSize=11;
    bank->LoadingQueue.Head.ResourceSize=22;
    owner.AddResource(bank->ResourceList.Head);
    owner.AddResource(bank->LoadingQueue.Head);
    CHECK(owner.ResourceCount==2 && owner.AllocatedMemory==33);
    delete bank;
    CHECK(owner.ResourceCount==0 && owner.AllocatedMemory==0 && allocations==3 && frees==3);
    CResourceList* heapList=new CResourceList;
    heapList->Head.ResourceSize=9;
    owner.AddResource(heapList->Head);
    CHECK(owner.ResourceCount==1 && owner.AllocatedMemory==9);
    delete heapList;
    CHECK(owner.ResourceCount==0 && owner.AllocatedMemory==0 && allocations==4 && frees==4);
    CHECK(owner.Head.NextResource==&owner.Head && owner.Head.PrevResource==&owner.Head);
    puts("RESOURCE_FAMILY_LIFETIME_PASS nativeRelease=1 flags0=1 bankDelete=1 listDelete=1 allocations=4 frees=4");
    return 0;
}

#include "../../src/compiled/00/42/CIVCountedPointeeBase_DeleteCIVCountedPointeeBaseUBEXXZ_0042d930.cpp"
#include "../../src/compiled/00/9f/CResourceList_RemoveResource_009fc020.cpp"
#include "../../src/compiled/00/9f/CResourceList_AddResource_009fbfe0.cpp"
