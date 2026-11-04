#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>
static unsigned int calls;
static bool refuse;
bool CResourceList::UnloadNextResource(unsigned long size) {
    if(size) return false;
    ++calls;
    if(refuse) return false;
    AllocatedMemory-=10;
    return true;
}
#define CHECK(x) do { if(!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    union { double align; unsigned char bytes[sizeof(CResourceList)]; } storage;
    memset(&storage,0,sizeof(storage));
    CResourceList* list=reinterpret_cast<CResourceList*>(storage.bytes);
    list->AllocatedMemory=50;list->ResourceCount=3;
    list->SetMaximumMemory(100);
    CHECK(list->MaximumMemory==100 && calls==0);
    list->SetMaximumMemory(30);
    CHECK(list->MaximumMemory==30 && list->AllocatedMemory==30 && calls==2);
    calls=0;refuse=true;
    list->SetMaximumMemory(0);
    CHECK(list->MaximumMemory==0 && list->AllocatedMemory==30 && calls==1);
    calls=0;refuse=false;list->ResourceCount=2;list->AllocatedMemory=100;
    list->SetMaximumMemory(0);
    CHECK(list->AllocatedMemory==80 && calls==2);
    calls=0;list->SetMaximumMemory(-1);
    CHECK(list->MaximumMemory==-1 && calls==0);
    puts("RESOURCE_LIST_MAXIMUM_PASS shrink=2 failure=1 attemptBound=1 unsignedLimit=1");
    return 0;
}

#include "test_support/resource_list_native_shrink_fixture.h"
