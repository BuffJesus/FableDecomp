#include "fable_resource_list.h"
#include <stdio.h>
#include <stdlib.h>
static unsigned int destroys,frees;
struct Probe : CIVCountedPointeeBase {
    virtual ~Probe() { ++destroys; }
    static void* operator new(size_t size) { return malloc(size); }
    static void operator delete(void* data) { ++frees;free(data); }
};
#define CHECK(x) do { if(!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    Probe* first=new Probe;
    CHECK(first->Release()==0 && destroys==1 && frees==1);
    Probe* shared=new Probe;
    shared->AddRef();
    CHECK(shared->Release()==1 && destroys==1 && frees==1);
    CHECK(shared->Release()==0 && destroys==2 && frees==2);
    const CIVCountedPointeeBase* direct=new Probe;
    direct->DeleteCIVCountedPointeeBase();
    CHECK(destroys==3 && frees==3);
    puts("CIV_COUNTED_DELETE_PASS lastOwner=1 sharedOwner=1 virtualDestructor=3 deallocations=3");
    return 0;
}
