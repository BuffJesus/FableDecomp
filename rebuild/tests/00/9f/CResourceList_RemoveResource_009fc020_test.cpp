#include "fable_resource_list.h"
#include <stdio.h>
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
bool CResourceList::HandleFailedAllocation(unsigned long) { return false; }
#define CHECK(x) do { if(!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    CResourceList list;
    CResource a,b,c;
    a.ResourceSize=10; b.ResourceSize=20; c.ResourceSize=30;
    list.AddResource(a); list.AddResource(b); list.AddResource(c);
    CHECK(list.ResourceCount==3 && list.AllocatedMemory==60);
    list.RemoveResource(b);
    CHECK(!b.ResourceList && !b.PrevResource && !b.NextResource);
    CHECK(a.NextResource==&c && c.PrevResource==&a);
    CHECK(list.ResourceCount==2 && list.AllocatedMemory==40);
    list.RemoveResource(a); list.RemoveResource(c);
    CHECK(!list.ResourceCount && !list.AllocatedMemory);
    CHECK(list.Head.NextResource==&list.Head && list.Head.PrevResource==&list.Head);
    // The native null-next path leaves PrevResource unchanged but accounts removal.
    a.ResourceList=&list; a.PrevResource=&list.Head; a.NextResource=0;
    list.ResourceCount=1;list.AllocatedMemory=10;
    list.RemoveResource(a);
    CHECK(!a.ResourceList && a.PrevResource==&list.Head && !a.NextResource);
    CHECK(!list.ResourceCount && !list.AllocatedMemory);
    // A broken back-link clears the removed node without repairing its neighbors.
    for(int mismatch=0;mismatch<2;++mismatch) {
        a.ResourceList=&list;a.PrevResource=&list.Head;a.NextResource=&list.Head;
        list.Head.NextResource=mismatch ? &a : &list.Head;
        list.Head.PrevResource=&list.Head;
        list.ResourceCount=1;list.AllocatedMemory=10;
        CResource* oldNext=list.Head.NextResource;
        list.RemoveResource(a);
        CHECK(!a.ResourceList && !a.PrevResource && !a.NextResource);
        CHECK(list.Head.NextResource==oldNext && list.Head.PrevResource==&list.Head);
        CHECK(!list.ResourceCount && !list.AllocatedMemory);
        list.Head.NextResource=&list.Head;
    }
    // A real resource destructor removes its attached node through this body.
    {
        CResource attached;
        attached.ResourceSize=77;
        list.AddResource(attached);
        CHECK(list.ResourceCount==1 && list.AllocatedMemory==77);
    }
    CHECK(!list.ResourceCount && !list.AllocatedMemory);
    CHECK(list.Head.NextResource==&list.Head && list.Head.PrevResource==&list.Head);
    puts("RESOURCE_LIST_REMOVE_PASS middle=1 ends=2 nullNext=1 brokenLinks=2 destructor=1");
    return 0;
}

#include "test_support/resource_list_native_lifetime_fixture.h"
