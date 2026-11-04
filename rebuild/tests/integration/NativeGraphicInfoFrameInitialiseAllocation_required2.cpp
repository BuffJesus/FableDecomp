#include <stdlib.h>
#include <new>
#include "fable_resource_list.h"
extern "C" __declspec(noinline) void RequiredAllocationProvider(CResource* resource,unsigned long size){resource->SetResourceSize(size);}
extern "C" void UnusedAllocationBoundary(){abort();}
int main(int,char**argv){RequiredAllocationProvider(reinterpret_cast<CResource*>(argv),1ul);return 0;}
#pragma comment(linker,"/include:?SetResourceSize@CResource@@QAEXK@Z")

