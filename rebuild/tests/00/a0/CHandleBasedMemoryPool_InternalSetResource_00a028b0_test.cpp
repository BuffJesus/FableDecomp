#include <stdio.h>
struct HandleAllocation { void* Resource; };
struct HandleInfo { HandleAllocation* Allocation; };
struct HandleArray { HandleInfo* Data; HandleInfo* End; HandleInfo* CapacityEnd; void* Allocator; };
struct Pool { unsigned char prefix[0x438]; HandleArray Handles;
 void InternalSetResource(long,void*); };
void Pool::InternalSetResource(long handle,void* resource) { Handles.Data[handle].Allocation->Resource=resource; }
int main() { HandleAllocation allocations[3]; HandleInfo entries[3]; Pool pool; pool.Handles.Data=entries;
 entries[0].Allocation=&allocations[0]; entries[1].Allocation=&allocations[1]; entries[2].Allocation=&allocations[2];
 allocations[0].Resource=0; allocations[1].Resource=0; allocations[2].Resource=0;
 pool.InternalSetResource(1,(void*)0x1234);
 if(allocations[0].Resource!=0 || allocations[1].Resource!=(void*)0x1234 || allocations[2].Resource!=0) return 1;
 printf("HANDLE_POOL_SET_RESOURCE PASS\n"); return 0; }