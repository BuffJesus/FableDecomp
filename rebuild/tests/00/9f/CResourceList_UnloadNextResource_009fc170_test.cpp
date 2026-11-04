#include "fable_resource_list.h"
#include <stdio.h>
static unsigned int calls,deletes;
static bool refValid=true;
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const { delete this; }
bool CResourceList::HandleFailedAllocation(unsigned long) { return false; }
struct Probe : CResource {
    int action;
    CResourceList* destination;
    Probe(int mode=0):action(mode),destination(0) { ResourceSize=10; }
    virtual ~Probe() { ++deletes; }
    virtual bool UnloadResource() {
        ++calls;refValid=refValid && IVRefCount==2;
        if(action==2) Release();
        if(action==3) { ResourceList->RemoveResource(*this); destination->AddResource(*this); }
        if(action==4) ResourceList->RemoveResource(*this);
        return action!=0;
    }
};
#define CHECK(x) do { if(!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    {
        CResourceList list;
        CHECK(!list.UnloadNextResource(0));
        Probe a,b;list.AddResource(a);list.AddResource(b);
        list.UnloadedThisFrame=2;
        CHECK(!list.UnloadNextResource(0) && calls==0);
        list.UnloadedThisFrame=0;list.CurrentFrame=0;
        CHECK(list.UnloadNextResource(0) && calls==0);
        CHECK(list.Head.NextResource==&b && list.Head.PrevResource==&a && list.UnloadedThisFrame==1);
        list.CurrentFrame=10;
        CHECK(list.UnloadNextResource(11) && calls==0);
        CHECK(list.Head.NextResource==&a && list.Head.PrevResource==&b && list.UnloadedThisFrame==2);
    }
    {
        CResourceList list;Probe a(0),b(1);list.AddResource(a);list.AddResource(b);list.CurrentFrame=10;
        CHECK(list.UnloadNextResource(10) && calls==1 && a.IVRefCount==1 && a.LastUsedFrame==0);
        CHECK(list.UnloadNextResource(0) && calls==2 && b.IVRefCount==1 && b.LastUsedFrame==10);
        CHECK(list.UnloadedThisFrame==2 && refValid);
    }
    {
        CResourceList list;Probe* dying=new Probe(2);list.AddResource(*dying);list.CurrentFrame=10;
        unsigned int before=deletes;
        CHECK(list.UnloadNextResource(0) && deletes==before+1 && list.ResourceCount==0 && list.AllocatedMemory==0);
        CHECK(list.UnloadedThisFrame==0 && list.Head.NextResource==&list.Head && refValid);
    }
    {
        CResourceList first,second;Probe moving(3);moving.destination=&second;
        first.AddResource(moving);first.CurrentFrame=10;
        CHECK(first.UnloadNextResource(0) && moving.ResourceList==&second && moving.IVRefCount==1);
        CHECK(first.ResourceCount==0 && first.UnloadedThisFrame==0 && second.ResourceCount==1);
    }
    {
        CResourceList list;Probe detached(4);list.AddResource(detached);list.CurrentFrame=10;
        CHECK(list.UnloadNextResource(0) && !detached.ResourceList && detached.IVRefCount==1);
        CHECK(list.ResourceCount==0 && list.UnloadedThisFrame==0);
    }
    {
        CResourceList list;Probe wrapped(1);list.CurrentFrame=0xfffffffe;list.AddResource(wrapped);
        list.CurrentFrame=1;list.UnloadDelay=3;
        CHECK(list.UnloadNextResource(0) && wrapped.LastUsedFrame==1 && refValid);
    }
    puts("RESOURCE_UNLOAD_PASS empty=1 capped=1 young=1 small=1 callback=2 deleted=1 transferred=1 detached=1 frameWrap=1");
    return 0;
}

#include "test_support/resource_list_native_lifetime_fixture.h"
#include "test_support/resource_list_native_remove_fixture.h"
